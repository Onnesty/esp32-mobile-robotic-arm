#include <Servo.h>

#define XAxisServoPin 11
#define YAxisServoPin 10
#define ClawServoPin 9

#define XLeftPin 8
#define XRightPin 7
#define YUpPin 6
#define YDownPin 5
#define ClawOpenPin 4
#define ClawClosePin 3

#define StepIncrement 2 // How many degrees per step

Servo XAxisServo;
Servo YAxisServo;
Servo ClawServo;

#define XPosLowerLimit 0
#define XPosUpperLimit 180

#define YPosLowerLimit 0
#define YPosUpperLimit 160

#define CPosLowerLimit 10
#define CPosUpperLimit 70

// Midpoint default positions, the claw default is closed position which is the upper limit
int XPos = (XPosUpperLimit + XPosLowerLimit) / 2;
int YPos = (YPosUpperLimit + YPosLowerLimit) / 2;
int CPos = CPosUpperLimit;


void setup() {
  
  XAxisServo.attach(XAxisServoPin);
  XAxisServo.write(XPos);

  delay(50);

  YAxisServo.attach(YAxisServoPin);
  YAxisServo.write(YPos);

  delay(50);

  ClawServo.attach(ClawServoPin);
  ClawServo.write(CPos);

  

  pinMode(XLeftPin, INPUT);
  pinMode(XRightPin, INPUT);

  pinMode(YUpPin, INPUT);
  pinMode(YDownPin, INPUT);
  pinMode(ClawOpenPin, INPUT);
  pinMode(ClawClosePin, INPUT);


}

void loop() {
  
  /** Currently not used for ICPEP competition

  if ( digitalRead(XLeftPin) == HIGH ) {
    
    XPos += StepIncrement;
    if (XPos > XPosUpperLimit) XPos = XPosUpperLimit;
    XAxisServo.write(XPos);

  } else if ( digitalRead(XRightPin) == HIGH) {

    XPos -= StepIncrement;
    if (XPos < XPosLowerLimit) XPos = XPosLowerLimit;
    XAxisServo.write(XPos);
    
  } else 
  
  */
  
  if ( digitalRead(YUpPin) == HIGH ) {
    
    YPos += StepIncrement;
    if (YPos > YPosUpperLimit) YPos = YPosUpperLimit;
    YAxisServo.write(YPos);

  } else if ( digitalRead(YDownPin) == HIGH ) {

    YPos -= StepIncrement;
    if (YPos < YPosLowerLimit) YPos = YPosLowerLimit;
    YAxisServo.write(YPos);

  } else if ( digitalRead(ClawOpenPin) == HIGH ) {

    CPos -= StepIncrement;
    if (CPos < CPosLowerLimit) CPos = CPosLowerLimit;
    ClawServo.write(CPos);

  } else if ( digitalRead(ClawClosePin) == HIGH ) {

    CPos += StepIncrement;
    if (CPos > CPosUpperLimit) CPos = CPosUpperLimit;
    ClawServo.write(CPos);

  } 

  delay(20);


}













