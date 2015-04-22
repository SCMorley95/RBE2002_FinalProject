#include <Servo.h>
#include <Serial.h>
#include <ultrasonic.h>

#define US_PING //All of the digital / analog inputs and outputs need to be entered
#define US_ECHO
#define STEPPER_STEP
#define STEPPER_DIR
#define SERVO_LEFT
#define SERVO_RIGHT
#define FLAME_IN
#define FAN_OUT
#define LINE_SENSOR_IN
#define CYCLE_BUTTON_IN

#define HALT 90
#define LEFT_FWD  //Decide on a speed for the motor
#define LEFT_BKWD
#define RIGHT_FWD
#define RIGHT_BKWD

Servo leftServo;
Servo rightServo;

boolean isDebugSonar;
boolean isDebugStepper;
boolean isDebugServo;
boolean isDebugFlame;
boolean isDebugFan;
boolean isDebugLineSensor;

boolean stepDir = 0;

State state;

void setup() {
	ultrasonic myUltraSonic(US_PING, US_ECHO);

	Serial.begin(9600);
	pinMode(US_PING, OUTPUT);
	pinMode(US_ECHO, INPUT);
	pinMode(STEPPER_STEP, OUTPUT);
	pinMode(STEPPER_DIR, OUTPUT);
	pinMode(SERVO_LEFT, OUTPUT);
	pinMode(FLAME_IN, INPUT);
	pinMode(LINE_SENSOR_IN, INPUT);
	pinMode(CYCLE_BUTTON_IN, INPUT_PULLUP);

	leftServo.attach(SERVO_LEFT);
	rightServo.attach(SERVO_RIGHT);

	attachInterrupt(CYCLE_BUTTON_IN, cycleISR, FALLING);

	Serial.println("--------DEBUG MODE ENABLED--------");
}
void loop() {
	switch (state) {
		case 0:
			if(isDebugSonar) {
				debugSonar();
				delay(DEBUG_PERIOD);
			}
			break;
		case 1:
			if(isDebugStepper) {
				debugStepper();
				delay(DEBUG_PERIOD);
			}
			break;
		case 2:
			if(isDebugServo) {
				debugServo();
				delay(DEBUG_PERIOD);
			}
			break;
		case 3:
			if(isDebugFlame) {
				debugFlame();
				delay(DEBUG_PERIOD);
			}
			break;
		case 4:
			if(isDebugFan) {
				debugFan();
				delay(DEBUG_PERIOD);
			}
			break;
		case 5:
			if(isDebugLineSensor) {
				debugLineSensor();
				delay(DEBUG_PERIOD);
			}
			break;
		case default:
			break;
	}
}
void debugSonar() {
	Serial.println("-----------SONAR DEBUG------------");
	float dist = myUltraSonic.distance();
	Serial.println("DISTANCE READING: %d", dist);
	Serial.println("----------------------------------");
}
void debugStepper() {
	Serial.println("-----------STEPPER DEBUG----------");
	Serial.println("-----------STEPPING FORWARDS------");
	stepMotorTest();
	Serial.println("------------STEPPING BACKWARDS----");
	changeStepDir();
	stepMotorTest();
	Serial.println("----------------------------------");
}
void debugServo() {
	Serial.println("------------SERVO DEBUG-----------");
	Serial.println("------------TESTING FORWARDS------");
	leftServo.write(LEFT_FWD);
	rightServo.write(RIGHT_FWD);
	delay(1000);
	leftServo.write(HALT);
	rightServo.write(HALT);
	delay(200);
	Serial.println("------------TESTING BACKWARDS-----");
	leftServo.write(LEFT_BKWD);
	rightServo.write(RIGHT_BKWD);
	delay(1000);
	leftServo.write(HALT);
	rightServo.write(HALT);
	delay(200);
	Serial.println("------------TESTING RIGHT TURN----");
	leftServo.write(LEFT_FWD);
	rightServo.write(RIGHT_BKWD);
	delay(1000);
	leftServo.write(HALT);
	rightServo.write(HALT);
	delay(200);
	Serial.println("-----------TESTING LEFT TURN------");
	leftServo.write(LEFT_BKWD);
	rightServo.write(RIGHT_FWD);
	delay(1000);
	leftServo.write(HALT);
	rightServo.write(HALT);
	delay(200);
	Serial.println("---------------------------------")
}
void debugFlame() {
	Serial.println("---------FLAME SENSOR DEBUG------");
	int temp = analogRead(FLAME_IN);
	Serial.println("FLAME READING: %d", temp);
	Serial.println("---------------------------------");
}
void debugLineSensor() {
	Serial.println("---------LINE SENSOR DEBUG-------");
	int line = analogRead(LINE_SENSOR_IN);
	Serial.println("LINE SENSOR READING: %d", line);
	Serial.println("---------------------------------");
}
void changeStepDir() {
	stepDir = !stepDir;
	digitalWrite(STEPPER_DIR, stepDir);
}
void stepMotorTest() {
	for(i=0;i<10;i++) {
		digitalWrite(STEPPER_STEP, HIGH);
		delay(50);
		digitalWrite(STEPPER_STEP, LOW);
		delay(100);
	}
}
void cycleISR() {
	boolean isButtonPressed = digitalRead(CYCLE_BUTTON_IN);
	//IF THE BUTTON IS PUSHED CYCLE STATES
}