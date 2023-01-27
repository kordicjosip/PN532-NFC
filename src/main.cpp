#include <Arduino.h>

void setup() {
  pinMode(23, OUTPUT);
}

void loop() {
  digitalWrite(23, HIGH); // turn the LED on
  delay(1000);             // wait for 500 milliseconds
  digitalWrite(23, LOW);  // turn the LED off
  delay(4000);             // wait for 500 milliseconds
}