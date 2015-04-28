#include <Encoder.h>
#include <Servo.h>

#include <Servo.h>
#include <Encoder.h>

#define LEFT_SERVO 10
#define RIGHT_SERVO 11

Servo LMotor;
Servo RMotor;
Encoder lEncoder(2, 3);
Encoder rEncoder(18, 19);
long oldPositionL = -999;
long oldPositionR = -999;

void setup() {
	Serial.begin(9600);
	pinMode(12, OUTPUT);
	LMotor.attach(LEFT_SERVO, 1000, 2000);
	RMotor.attach(RIGHT_SERVO, 1000, 2000);
	lEncoder.write(0);
	rEncoder.write(0);
}
void loop() {
	digitalWrite(12, HIGH);
	LMotor.write(110);
	RMotor.write(70);
	long newPositionL = lEncoder.read();
	long newPositionR = rEncoder.read();
	if (newPositionL != oldPositionL) {
    	oldPositionL = newPositionL;
    	Serial.print(newPositionL);
    	Serial.print("  ");
  }
  if(newPositionR != oldPositionR) {
  	oldPositionR = newPositionR;
  	Serial.println(newPositionR);
  }
}