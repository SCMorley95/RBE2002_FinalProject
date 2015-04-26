/* TODO:
Get PID implemented
Debug stuff
Get LCD implemented
Get Accelerometer implemented
*/

#include <TimerOne.h>
#include <PID_v1.h>
#include <Servo.h>
#include <Encoder.h>
#include <ultrasonic.h>
#include <directionalArray.h>

#define US_PING 24
#define US_ECHO 25
#define STEPPER_STEP 22
#define STEPPER_DIR 23
#define SERVO_LEFT 0
#define SERVO_RIGHT 1
#define FLAME_IN 2
#define FAN_OUT 26
#define LINE_SENSOR_IN 3
#define CYCLE_BUTTON_IN 27
#define ENCODER_IN_LEFT_1 28
#define ENCODER_IN_LEFT_2
#define ENCODER_IN_RIGHT_1 29
#define ENCODER_IN_RIGHT_2
#define LIDAR_IN 30

#define US_DISTANCE_THRESHOLD
#define LIDAR_THRESHOLD
#define LINE_SENSOR_THRESHOLD
#define FLAME_THRESHOLD

#define HALT 90
#define LEFT_BKWD 60
#define LEFT_FWD 120
#define RIGHT_FWD 60
#define RIGHT_BKWD 120

#define WHEEL_CIRCUMFRENCE 8.6393725

boolean isTurretRotation = 1;
boolean turretDirection = 0;
boolean isStepping = 0

int turretPos = 0;

Servo motorLeft;
Servo motorRight;
Servo fanMotor;

int xPos = 0;
int yPos = 0;
directionalArray dirArray(0, 1);

Encoder encLeft (ENCODER_IN_LEFT_1, ENCODER_IN_LEFT_2);
Encoder encRight (ENCODER_IN_RIGHT_1, ENCODER_IN_RIGHT_2);

ultrasonic myUltraSonic (US_PING, US_ECHO);

void setup() {
	Timer1.initialize(STEPPER_PERIOD);
	Timer1.attachInterrupt(turretISR);
	motorLeft.attach(SERVO_LEFT);
	motorRight.attach(SERVO_RIGHT);
	fanMotor.attach(FAN_OUT);
}
void loop() {
	state = setObjective();
	if(isTurretRotation) {
		noInterrupts();
		boolean doStep = isStepping;
		isStepping = 0;
		interrupts();
		digitalWrite(STEPPER_STEP, doStep);
	}
	switch(state) {
		case default:
			break;
		case 0: //about to go off cliff
			setRobotSpeed(ROBOT_STOP);
			moveFwd(-10);
			turnDeg(-90);
			break;
		case 1: //flame detected
			stopRobot();
			isTurretRotation = false;
			fanMotor.write(180);
			delay(5000);
			reportLocation();
			break;
		case 2: //walls on front and right
			turnDeg(-90);
			break;
		case 3: //wall just on right
			setRobotSpeed(ROBOT_FULL_FORWARD);
			break;
		case 4: //wall just in front
			turnDeg(90);
			break;
		case 5: //no walls detected
			turnDeg(90);
			break;
		//etc
	}
}
int setObjective() {
	if(!checkLine()) {
		return 0;
	}
	if(isFlame()) {
		return 1;
	}
	else if(wallSide()) {
		if(wallFront()) {
			return 2;
		}
		else {
			return 3;
		}
	}
	else {
		if(wallFront()) {
			return 4;
		}
		else {
			return 5;
		}
	}
}
boolean checkLine() {
	return analogRead(LINE_SENSOR_IN) > LINE_SENSOR_THRESHOLD;
}
boolean wallFront() {
	return myUltraSonic.distance < US_DISTANCE_THRESHOLD;
}
boolean wallSide() {
	return analogRead(LIDAR_IN) < LIDAR_THRESHOLD;
}
boolean isFlame() {
	return analogRead(FLAME_IN) > FLAME_THRESHOLD;
}
void turretISR() {
	if(isTurretRotation) {
		isStepping = 1;
		turretPos++;
		if(turretPos >= TURRET_POS_MAX) {
			turretDirection = !turretDirection;
			turretPos = 0;
		}
	}
}
void moveFwd(int inches) {
	float deg = inchesToDeg(inches);
	int encoderOffset = encoderAvg();
	if(inches < 0) {
		while(encoderAvg() > deg + encoderOffset) {
			leftServo.write(LEFT_FWD);
			rightServo.write(RIGHT_FWD);
		}
	}
	else if(inches > 0) {
		while(encoderAvg() < deg + encoderOffset) {
			leftServo.write(LEFT_BKWD);
			rightServo.write(RIGHT_BKWD);
		}
	}
	leftServo.write(HALT);
	rightServo.write(HALT);
}
void setRobotSpeed(int robotSpeed) {
	switch (robotSpeed) {
		case 0: //stop
			leftServo.write(HALT);
			rightServo.write(HALT);
			break;
		case 1: //forwards
			leftServo.write(LEFT_FWD);
			rightServo.write(RIGHT_FWD);
			break;
		case 2: //backwards
			leftServo.write(LEFT_BKWD);
			rightServo.write(RIGHT_BKWD);
	}
}
float inchesToDeg(int inches) {
	return (float)inches /WHEEL_CIRCUMFRENCE;
}
float degToInches(int deg) {
	return (deg / 360)*WHEEL_CIRCUMFRENCE;
}
int encoderAvg() {
	return (encLeft.read() + encRight.read()) / 2;
}
void resetEncoders() {
	encLeft.write(0);
	encRight.write(1);
}
void posUpdate() {
	int distanceTravelled = degToInches(encoderAvg());
	xPos += distanceTravelled*dirArray.xval();
	yPos += distanceTravelled*dirArray.yval();
	resetEncoders();
}
void turnDeg(int deg) {
	posUpdate();
	dirArray.rotate(deg);
	//PID witchcraft goes here
}
void reportLocation() {
	//print xPos and yPos on the LCD display
}