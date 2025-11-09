#include <Arduino.h>

// Setup Pins numbers related to board
const int RAPin = 0; // Fill in later when we get the board
const int DecPin = 0; // Change pin later when we get the board

// our controller moves some angular distance X in arcsec per step
// this angular distance is decided by
int mechanical = 450;
int distanceStep = 1296000/(200 * mechanical * 32);
// and is dependent on the gear reduction provided by the planetary gearbox

// this is in steps per second
int RA = 15/distanceStep;


void setup() {
  // Setting pins to output
  pinMode(RAPin, OUTPUT);
  pinMode(DecPin, OUTPUT);
}

void loop() {
  digitalWrite(RAPin, HIGH);
  delayMicroseconds(1/RA); 
}