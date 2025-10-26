#include <Arduino.h>

// Setup Pins numbers related to board
const int dirpin = 0; // Fill in later when we get the board
const int steppin = 0; // Change pin later when we get the board

void setup() {
  // Setting pins to output
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}