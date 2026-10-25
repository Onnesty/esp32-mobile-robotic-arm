#include "AFMotor_R4.h"


// Create Motor Objects
AF_DCMotor Motor1(1); // NW
AF_DCMotor Motor2(2); // SW
AF_DCMotor Motor3(3); // NE
AF_DCMotor Motor4(4); // SE

// Define Speed
#define MotorSpeed 255

#define ForwardSignal A0
#define BackwardSignal A1
#define LeftSignal A2
#define RightSignal A3


void setup() {
  
  // Motor Initialization 
  Motor1.setSpeed(MotorSpeed);
  Motor2.setSpeed(MotorSpeed);
  Motor3.setSpeed(MotorSpeed);
  Motor4.setSpeed(MotorSpeed);

  Motor1.run(RELEASE);
  Motor2.run(RELEASE);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE);
  

  // Initialization of Analog Pins for Input
  // NOTE: Do not turn to OUTPUT since it will be receiving signals from an esp32 and will thus be connected to its digital pins, you will cook the esp
  pinMode(ForwardSignal, INPUT);
  pinMode(BackwardSignal, INPUT);
  pinMode(LeftSignal, INPUT);
  pinMode(RightSignal, INPUT);

  
  delay(1000);

}

void loop() {
  

  if (digitalRead(BackwardSignal) == HIGH) { // Backward
    
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD);

  } else if (digitalRead(ForwardSignal) == HIGH) { // Forward
    
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);

  } else if (digitalRead(LeftSignal) == HIGH) { // Left
    
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);

  } else if (digitalRead(RightSignal) == HIGH) { // Right
    
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD);

  } else { // Stop if no commands

    Motor1.run(RELEASE);
    Motor2.run(RELEASE);
    Motor3.run(RELEASE);
    Motor4.run(RELEASE);

  }
  
  delay(20);


  

}
