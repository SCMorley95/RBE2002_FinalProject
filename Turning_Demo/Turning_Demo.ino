#include <Servo.h>
#include <Encoder.h>
#include <ultrasonic.h>
#include <TimerOne.h>
#include <PID_v1.h>

#define LEFT_MOTOR_IN 11
#define RIGHT_MOTOR_IN 10
#define LEFT_FWD 108
#define RIGHT_FWD 75
#define LEFT_BKWD 72
#define RIGHT_BKWD 105
#define ENCODER_IN_LEFT_1 2
#define ENCODER_IN_LEFT_2 3
#define ENCODER_IN_RIGHT_1 18
#define ENCODER_IN_RIGHT_2 19
#define RIGHT 0
#define LEFT 1
#define US_PING 28
#define US_ECHO 29
#define US_PING_1 26
#define US_ECHO_1 27
#define TURN_TIME 750
#define HALT 90
#define US_THRESHOLD_FRONT 4
#define US_THRESHOLD_RIGHT 25
#define BUTTON_IN 22

ultrasonic ultraFront (US_PING, US_ECHO);
ultrasonic ultraRight (US_PING_1, US_ECHO_1);

Servo leftServo;
Servo rightServo;

Encoder leftEncoder(ENCODER_IN_LEFT_2, ENCODER_IN_LEFT_2);
Encoder rightEncoder(ENCODER_IN_RIGHT_1, ENCODER_IN_RIGHT_2);
/*
float rightDistLast = 0;
float rightDist = 0;
int timerCalibrator = 0;
*/
double Setpoint, Input, Output;
int leftMotorOffset = 0;
float rightDist = 0;

PID myPID(&Input, &Output, &Setpoint, 2, 3, 1, REVERSE);


void setup() {
	Serial.begin(9600);
	Input = ultraFront.distance();
	Setpoint = 4;
	myPID.SetMode(AUTOMATIC);
	myPID.SetOutputLimits(-5, 5);
	leftServo.attach(LEFT_MOTOR_IN);
	rightServo.attach(RIGHT_MOTOR_IN);
	pinMode(BUTTON_IN, INPUT_PULLUP);
	while(1) {
		if(!digitalRead(BUTTON_IN)) {
			break;
		}
	}
	delay(1000);
}
void loop() {
	Input = ultraRight.distance();
	myPID.Compute();
	Serial.print("leftMotorOffset:  ");
	Serial.println(Output);
	Serial.print("Input:  ");
	Serial.println(Input);
	leftMotorOffset = Output;
	//int state = getObjective();
	int state = 1;
	if(!digitalRead(BUTTON_IN)) {
		state = 4;
		delay(500);
	}
	Serial.print("State:   ");
	Serial.println(state);
	switch (state) {
		case 0: //Wall on just the front
			turn(RIGHT);
			break;
		case 1: //Wall on just the right
			leftServo.write(LEFT_FWD + leftMotorOffset);
			Serial.print(String(LEFT_FWD + leftMotorOffset));
			Serial.print(" ");
			rightServo.write(RIGHT_FWD + leftMotorOffset);
			Serial.println(String(RIGHT_FWD + leftMotorOffset));
			break;
		case 2: //Wall on front and right
			turn(LEFT);
			break;
		case 3: //No walls detected
			turn(RIGHT);
			break;
		case 4: //Button shutdown
			leftServo.write(HALT);
			rightServo.write(HALT);
			delay(1000);
			while(1) {
				if(!digitalRead(BUTTON_IN)) {
					break;
				}
			}
	}
	/*
	Serial.print(String(ultraFront.distance()));
	Serial.print("  ");
	Serial.print(String(ultraRight.distance()));
	Serial.print("  ");
	*/
}
void turn(int dir) {
	switch (dir) {
		case RIGHT:
			leftServo.write(LEFT_FWD + 5);
			rightServo.write(RIGHT_BKWD + 5);
			waitForEncoder(300);
			leftServo.write(HALT);
			rightServo.write(HALT);
			delay(100);
			leftServo.write(LEFT_FWD);
			rightServo.write(RIGHT_FWD);
			waitForEncoder(500);
			leftServo.write(HALT);
			rightServo.write(HALT);
			break;
		case LEFT:
			leftServo.write(LEFT_BKWD);
			rightServo.write(RIGHT_FWD);
			waitForEncoder(250);
			leftServo.write(HALT);
			rightServo.write(HALT);
			break;
	}
}
int getObjective() {
	if(!digitalRead(BUTTON_IN)) {
		return 4;
	}
	else if(isWallRight()) {
		if(isWallFront()) {
			return 2;
		}
		else {
			return 1;
		}
	}
	else if(isWallFront()) {
		return 0;
	}
	else {
		return 3;
	}
}
bool isWallRight() {
	rightDist = ultraRight.distance();
	return rightDist < US_THRESHOLD_RIGHT;
}
bool isWallFront() {
	float frontDist = ultraFront.distance();
	return frontDist < US_THRESHOLD_FRONT;
}
void waitForEncoder(int dist) {
	leftEncoder.write(0);
	int absDist = 0;
	while(absDist < dist) {
		absDist = abs(leftEncoder.read());
	}
}
/*
int calculateOffset() {
	noInterrupts();
	int offset;
	offset = ((rightDist - rightDistLast) * 10) * 10;
	if(abs(offset) > 20) {
		offset = 0;
	}
	rightDistLast = rightDist;
	interrupts();
	return offset;
}
void hundredMSISR() {
	leftMotorOffset = calculateOffset();
	Serial.print("leftMotorOffset:  ");
	Serial.println(String(leftMotorOffset));
}
*/