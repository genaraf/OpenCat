#include <M5Atom.h>

#include "M5Ultrasonic.h"

#define TRIGGER 32
#define ECHO 26

#define LONGEST_DISTANCE 200 // 200 cm = 2 meters

M5Ultrasonic::M5Ultrasonic(byte signal_pin, byte rgb_pin) {
  pinMode(TRIGGER, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT); // Sets the echoPin as an Input
  farTime =  LONGEST_DISTANCE*2/0.034;
}    

uint16_t M5Ultrasonic::GetUltrasonicDistance(void) {
  // Clears the trigPin
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(ECHO, HIGH, farTime);

  // Calculating the distance
  // 10^-6 * 34000 cm/s
  FrontDistance = (uint16_t)((float)duration * 0.034 / 2);    
  return FrontDistance;
}    

void M5Ultrasonic::SetRgbColor(E_RGB_INDEX index, long Color) {

    
}    

void M5Ultrasonic::SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect) {

    
}    
