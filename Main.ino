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
#include <directionalArray.h> //Custom library found in /Github/RBE2002_FinalProject/Libraries
#include <LiquidCrystal.h>

#define US_PING 28
#define US_ECHO 29
#define US_PING_1 26
#define US_ECHO_1 27
#define STEPPER_STEP 23
#define STEPPER_DIR 24
#define SERVO_LEFT 10
#define SERVO_RIGHT 11
#define FLAME_IN 10
#define FAN_OUT 25
#define LINE_SENSOR_IN 11
#define CYCLE_BUTTON_IN 22
#define ENCODER_IN_LEFT_1 2
#define ENCODER_IN_LEFT_2 3
#define ENCODER_IN_RIGHT_1 18
#define ENCODER_IN_RIGHT_2 19
#define STEPPER_5V_OUT 13
#define LCD_5V_OUT 12
#define LCD_1 14
#define LCD_2 15
#define LCD_3 16
#define LCD_4 17
#define LCD_5 18
#define LCD_6 19

#define US_DISTANCE_THRESHOLD 3
#define LINE_SENSOR_THRESHOLD 275
#define FLAME_THRESHOLD 300
#define STEPPER_PERIOD 1000000
#define TURRET_POS_MAX 50 //CHANGE THIS TO ACTUALLY BE 180 DEGREES
#define ENC_90_DEG 1000 //ALSO THIS IS ALSO DEFS NOT RIGHT I MADE IT UP HAHAHAHAHA
#define TURN_TIME 1000 //MORE MADE UP STUFF DID YOU EVEN SEE THIS COMING

#define ROBOT_STOP 0
#define ROBOT_FULL_FORWARD 1
#define ROBOT_FULL_REVERSE 2

#define HALT 90
#define LEFT_BKWD 60
#define LEFT_FWD 120
#define RIGHT_FWD 60
#define RIGHT_BKWD 120

#define WHEEL_CIRCUMFRENCE 8.6393725
#define PVAL 5
#define IVAL 1
#define DVAL 2

double Setpoint, Input, Output;
boolean isTurretRotation = 1;
boolean turretDirection = 0;
boolean isStepping = 0;
int turretPos = 0;
float gyroOffset;
Servo leftServo;
Servo rightServo;
int xPos = 0;
int yPos = 0;

directionalArray dirArray(0, 1);
Encoder encLeft (ENCODER_IN_LEFT_1, ENCODER_IN_LEFT_2);
Encoder encRight (ENCODER_IN_RIGHT_1, ENCODER_IN_RIGHT_2);
ultrasonic ultraF (US_PING, US_ECHO);
ultrasonic ultraR (US_PING_1, US_ECHO_1);
PID encPID(&Input, &Output, &Setpoint, PVAL, IVAL, DVAL, DIRECT);
LiquidCrystal lcd(LCD_1, LCD_2, LCD_3, LCD_4, LCD_5, LCD_6);

void setup() {
	lcd.begin(16,2);
	Serial.begin(9600);
	Timer1.initialize(STEPPER_PERIOD);
	Timer1.attachInterrupt(turretISR);
	leftServo.attach(SERVO_LEFT, 1000, 2000);
	rightServo.attach(SERVO_RIGHT, 1000, 2000);
	encPID.SetOutputLimits(0, 30);
	pinMode(STEPPER_5V_OUT, OUTPUT);
	pinMode(LCD_5V_OUT, OUTPUT);
	Serial.println("READY TO BEGIN, WAITING FOR INPUT");
	lcd.print("STANDBY");
	while(digitalRead(CYCLE_BUTTON_IN));
}
void loop() {
	digitalWrite(LCD_5V_OUT, HIGH);
	digitalWrite(STEPPER_5V_OUT, HIGH);
	digitalWrite(STEPPER_DIR, turretDirection);
	int state = setObjective();
	if(isTurretRotation) {
		noInterrupts();
		boolean doStep = isStepping;
		isStepping = 0;
		interrupts();
		digitalWrite(STEPPER_STEP, doStep);
	}
	switch(state) {
		case 0: //about to go off cliff
			setRobotSpeed(ROBOT_STOP);
			reverseForMS(500);
			turnDeg(-90);
			break;
		case 1: //flame detected
			setRobotSpeed(ROBOT_STOP);
			isTurretRotation = false;
			digitalWrite(FAN_OUT, HIGH);
			Serial.println("SHIT FIRE AAAAAAAAAAAAAAA");
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
	Serial.print(" State: ");
	Serial.print(String(state));
	Serial.print(" Flame Sensor: ");
	Serial.print(analogRead(FLAME_IN));
	Serial.print(" PID Output: ");
	Serial.println(Output);
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
	return ultraF.distance() < US_DISTANCE_THRESHOLD;
}
boolean wallSide() {
	return ultraR.distance() < US_DISTANCE_THRESHOLD;
}
boolean isFlame() {
	return analogRead(FLAME_IN) < FLAME_THRESHOLD;
}
void turretISR() {
	if(isTurretRotation) {
		Serial.println("MADE STEPPIN YO");
		isStepping = 1;
		turretPos++;
		if(turretPos >= TURRET_POS_MAX) {
			turretDirection = !turretDirection;
			turretPos = 0;
		}
	}
}
void reverseForMS(int MS) {
	setRobotSpeed(ROBOT_FULL_REVERSE);
	delay(MS);
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
	encRight.write(0);
}
void posUpdate() {
	int distanceTravelled = degToInches(encoderAvg());
	xPos += distanceTravelled*dirArray.xval();
	yPos += distanceTravelled*dirArray.yval();
	resetEncoders();
}
/*
void turnDeg(int deg) {
	posUpdate();
	dirArray.rotate(deg);
	switch (deg) {
		case -90: //LEFT
			while(Output > 1) {
				Setpoint = ENC_90_DEG;
				Input = encRight.read();
				encPID.Compute();
				leftServo.write(90 + Output);
				rightServo.write(90 - Output);
			}
			break;
		case 90: //RIGHT
			while(Output > 1) {
				Setpoint = ENC_90_DEG;
				Input = encLeft.read();
				encPID.Compute();
				leftServo.write(90 - Output);
				rightServo.write(90 + Output);
			}
			break;
	}
}
*/
void turnDeg(int deg) {
	switch(deg) {
		case 90:
			Serial.println("TURNING RIGHT WOOOOOOOO");
			leftServo.write(LEFT_FWD);
			rightServo.write(RIGHT_BKWD);
			delay(TURN_TIME);
			break;
		case -90:
			Serial.println("OH SHIT LEFT WHAT");
			leftServo.write(LEFT_BKWD);
			rightServo.write(RIGHT_FWD);
			delay(TURN_TIME);
			break;
	}
}
void reportLocation() {
	lcd.clear();
	lcd.print("X: ");
	lcd.setCursor(3,0);
	lcd.print(xPos);
	lcd.setCursor(0,1);
	lcd.print("Y: ");
	lcd.setCursor(3,1);
	lcd.print(yPos);
}