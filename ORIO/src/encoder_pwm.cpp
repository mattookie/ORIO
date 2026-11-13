#include "encoder_pwm.h"

#include <math.h>

namespace {

constexpr uint16_t PWM_COUNTS_PER_REV = 4096;
constexpr uint16_t PWM_HALF_RANGE = PWM_COUNTS_PER_REV / 2;

// AS5048A PWM frame:
// 12 init clocks
// 4 error-status clocks
// 4095 angle-data clocks
// 8 exit clocks
constexpr float PWM_FRAME_CLOCKS = 4119.0f;

// During a normal frame, the first 16 clocks are HIGH
// before the angle-dependent portion.
constexpr float PWM_FIXED_HIGH_CLOCKS = 16.0f;

constexpr float DEGREES_PER_COUNT =
    360.0f / PWM_COUNTS_PER_REV;

}

AS5048APWM::AS5048APWM(uint8_t pwmPin)
    : _pwmPin(pwmPin),
      _havePreviousReading(false),
      _previousCount(0),
      _relativeCount(0) {
}

void AS5048APWM::begin() {
    pinMode(_pwmPin, INPUT);

    _havePreviousReading = false;
    _previousCount = 0;
    _relativeCount = 0;
}

bool AS5048APWM::read(
    EncoderPWMReading& reading,
    uint32_t timeoutUs
) {
    reading = {};

    // Measure the HIGH and LOW portions of the PWM signal.
    const uint32_t highTimeUs =
        pulseIn(_pwmPin, HIGH, timeoutUs);

    const uint32_t lowTimeUs =
        pulseIn(_pwmPin, LOW, timeoutUs);

    if (highTimeUs == 0 || lowTimeUs == 0) {
        return false;
    }

    const uint32_t periodUs =
        highTimeUs + lowTimeUs;

    if (periodUs == 0) {
        return false;
    }

    // Measure duty cycle instead of assuming an exact 1 kHz
    // frequency. The AS5048A oscillator has tolerance.
    const float highFraction =
        static_cast<float>(highTimeUs) /
        static_cast<float>(periodUs);

    // Convert the duty cycle back into the AS5048A's
    // internal PWM clock units.
    const float measuredHighClocks =
        highFraction * PWM_FRAME_CLOCKS;

    const float dataClocks =
        measuredHighClocks - PWM_FIXED_HIGH_CLOCKS;

    // During an AS5048A system error, the data section is
    // forced to zero and Error_n changes the expected pulse.
    if (dataClocks < -1.0f) {
        reading.sensorError = true;
        reading.highTimeUs = highTimeUs;
        reading.periodUs = periodUs;

        return false;
    }

    long decodedCount =
        lroundf(dataClocks);

    if (decodedCount < 0) {
        decodedCount = 0;
    }
    else if (decodedCount > 4095) {
        decodedCount = 4095;
    }

    const uint16_t currentCount =
        static_cast<uint16_t>(decodedCount);

    int32_t delta = 0;

    if (_havePreviousReading) {

        delta =
            static_cast<int32_t>(currentCount) -
            static_cast<int32_t>(_previousCount);

        // Correct forward/backward wraparound at 0/4095.
        if (delta > PWM_HALF_RANGE) {
            delta -= PWM_COUNTS_PER_REV;
        }
        else if (
            delta <
            -static_cast<int32_t>(PWM_HALF_RANGE)
        ) {
            delta += PWM_COUNTS_PER_REV;
        }

        _relativeCount += delta;
    }
    else {
        // First reading simply establishes our reference.
        _havePreviousReading = true;
    }

    _previousCount = currentCount;

    reading.valid = true;
    reading.sensorError = false;

    reading.count = currentCount;

    reading.angleDegrees =
        static_cast<float>(currentCount) *
        DEGREES_PER_COUNT;

    reading.deltaCount =
        static_cast<int16_t>(delta);

    reading.deltaDegrees =
        static_cast<float>(delta) *
        DEGREES_PER_COUNT;

    reading.relativeDegrees =
        static_cast<float>(_relativeCount) *
        DEGREES_PER_COUNT;

    reading.highTimeUs = highTimeUs;
    reading.periodUs = periodUs;

    return true;
}

void AS5048APWM::resetRelativePosition() {
    _relativeCount = 0;
}
