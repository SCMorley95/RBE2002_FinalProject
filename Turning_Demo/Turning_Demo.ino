#include <Servo.h>
#include <Encoder.h>
#include <ultrasonic.h>
#include <TimerOne.h>
#include <PID_v1.h>
#include <LiquidCrystal.h>

#define LEFT_MOTOR_IN 11
#define RIGHT_MOTOR_IN 10
#define LEFT_FWD 108
#define RIGHT_FWD 79
#define LEFT_BKWD 79
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
#define US_THRESHOLD_FRONT 5
#define US_THRESHOLD_RIGHT 25
#define BUTTON_IN 22
#define STEPPER_STEP 23
#define STEPPER_DIR 24
#define STEPPER_5V_OUT A1
#define FAN_5V_OUT A0
#define LCD_5V_OUT A2
#define FAN_OUT 25
#define FLAME_IN A4
#define STEPPER_ENABLE 12
#define LINE_SENSOR_IN A11

boolean stepperDir = 1;
int steps = 0;
int fireSensor;

ultrasonic ultraFront (US_PING, US_ECHO);
ultrasonic ultraRight (US_PING_1, US_ECHO_1);

Servo leftServo;
Servo rightServo;

LiquidCrystal myLCD(9, 8, 7, 6, 5, 4);

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

//PID myPID(&Input, &Output, &Setpoint, 2, 3, 2, REVERSE);
PID myPID(&Input, &Output, &Setpoint, 3, 1.5, 5, REVERSE);

void setup() {
	noInterrupts();
	myLCD.begin(16,2);
	Serial.begin(9600);
	Input = ultraFront.distance();
	Setpoint = 5;
	myPID.SetMode(AUTOMATIC);
	myPID.SetOutputLimits(-2.25, 2.25);
	leftServo.attach(LEFT_MOTOR_IN);
	rightServo.attach(RIGHT_MOTOR_IN);

	pinMode(LINE_SENSOR_IN, INPUT);
	pinMode(STEPPER_STEP, OUTPUT);
	pinMode(STEPPER_DIR, OUTPUT);
	pinMode(STEPPER_5V_OUT, OUTPUT);
	pinMode(FAN_5V_OUT, OUTPUT);
	pinMode(BUTTON_IN, INPUT_PULLUP);
	pinMode(FLAME_IN, INPUT);
	pinMode(STEPPER_ENABLE, OUTPUT);
	pinMode(LCD_5V_OUT, OUTPUT);
	digitalWrite(LCD_5V_OUT, HIGH);
	digitalWrite(STEPPER_5V_OUT, HIGH);
	Timer1.initialize(100000);
	Timer1.attachInterrupt(turretISR);
	digitalWrite(FAN_5V_OUT, HIGH);
	digitalWrite(FAN_OUT, LOW);
	digitalWrite(STEPPER_ENABLE, LOW);
	myLCD.print("Noice");
	while(1) {
		if(!digitalRead(BUTTON_IN)) {
			interrupts();
			break;
		}
	}
	delay(1000);
}
void loop() {
	int leftIn = leftEncoder.read();
	int rightIn = rightEncoder.read();
	Input = ultraRight.distance();
	myPID.Compute();
	Serial.print("leftMotorOffset:  ");
	Serial.println(Output);
	Serial.print("Input:  ");
	Serial.println(Input);
	leftMotorOffset = Output;
	int state = getObjective();
	//int state = 5;
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
			while(1);
		case 5:
			int lineIn = analogRead(LINE_SENSOR_IN);
			if(lineIn > 750) {
				leftServo.write(LEFT_BKWD);
				rightServo.write(RIGHT_BKWD);
				delay(200);
				leftServo.write(HALT);
				rightServo.write(HALT);
				while(1);
			}
			else {
				leftServo.write(LEFT_FWD);
				rightServo.write(RIGHT_FWD);
			}
			break;
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
		/*
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
			*/
			break;
		case LEFT:
			leftServo.write(LEFT_BKWD + 5);
			rightServo.write(RIGHT_FWD + 5);
			waitForEncoder(200);
			leftServo.write(HALT);
			rightServo.write(HALT);
			delay(200);
			break;
	}
}
int getObjective() {
	int lineSensor = analogRead(LINE_SENSOR_IN);
	if(lineSensor > 750) {
		leftServo.write(HALT);
		rightServo.write(HALT);
		return 2;
	}
	else if(fireSensor < 200 && fireSensor != 0) {
		noInterrupts();
		digitalWrite(STEPPER_STEP, LOW);
		digitalWrite(FAN_OUT, HIGH);
		digitalWrite(STEPPER_ENABLE, HIGH);
		leftServo.write(HALT);
		rightServo.write(HALT);
		while(1) {
			if(!digitalRead(BUTTON_IN)) {
				digitalWrite(FAN_OUT, LOW);
				while(1);
			}
		}
	}
	else if(!digitalRead(BUTTON_IN)) {
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
void turretISR() {
	digitalWrite(STEPPER_STEP, HIGH);
	digitalWrite(STEPPER_DIR, stepperDir);
	fireSensor = analogRead(FLAME_IN);
	steps++;
	if(steps >= 50) {
		stepperDir = !stepperDir;
		steps = 0;
	}
	digitalWrite(STEPPER_STEP, LOW);
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