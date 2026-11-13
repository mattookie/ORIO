#pragma once

#include <Arduino.h>

// One decoded AS5048A PWM measurement.
struct EncoderPWMReading {
    bool valid;
    bool sensorError;

    uint16_t count;          // 0-4095 PWM angle count
    float angleDegrees;      // Current shaft angle, 0 <= angle < 360

    int16_t deltaCount;      // Movement since previous valid reading
    float deltaDegrees;      // Movement since previous valid reading
    float relativeDegrees;   // Total movement since startup/reset

    uint32_t highTimeUs;     // Measured PWM HIGH time
    uint32_t periodUs;       // Measured complete PWM period
};

class AS5048APWM {
public:
    explicit AS5048APWM(uint8_t pwmPin);

    // Configure the ESP32 GPIO receiving the encoder PWM signal.
    void begin();

    // Read and decode one PWM frame.
    bool read(EncoderPWMReading& reading, uint32_t timeoutUs = 3000);

    // Reset accumulated relative movement to zero.
    void resetRelativePosition();

private:
    uint8_t _pwmPin;

    bool _havePreviousReading;
    uint16_t _previousCount;
    int32_t _relativeCount;
};
