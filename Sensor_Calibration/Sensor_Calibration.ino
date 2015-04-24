#include <Servo.h>
//#include <Serial.h>
#include <ultrasonic.h>

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
#define ENCODER_IN_LEFT 28
#define ENCODER_IN_RIGHT 29

#define HALT 90
#define LEFT_BKWD 60
#define LEFT_FWD 120
#define RIGHT_FWD 60
#define RIGHT_BKWD 120
#define DEBUG_PERIOD 200

Servo leftServo;
Servo rightServo;

boolean isDebugSonar = 1;
boolean isDebugStepper = 0;
boolean isDebugServo = 0;
boolean isDebugFlame = 0;
boolean isDebugFan = 0;
boolean isDebugLineSensor = 1;

boolean stepDir = 0;

int state;

ultrasonic myUltraSonic (US_PING, US_ECHO);

void setup() {

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
				digitalWrite(FAN_OUT, LOW);
			}
			break;
		case 5:
			if(isDebugLineSensor) {
				debugLineSensor();
				delay(DEBUG_PERIOD);
			}
			break;
	}
	state++;
	if(state >= 6) {
		state = 0;
	}
}
void debugSonar() {
	Serial.println("-----------SONAR DEBUG------------");
	float distFloat = myUltraSonic.distance();
	String dist = String(distFloat);
	Serial.print("DISTANCE READING:");
	Serial.println(dist);
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
	Serial.println("---------------------------------");
}
void debugFlame() {
	Serial.println("---------FLAME SENSOR DEBUG------");
	String temp = String(analogRead(FLAME_IN));
	Serial.print("FLAME READING: ");
	Serial.println(temp);
	Serial.println("---------------------------------");
}
void debugFan() {
	Serial.println("----------TESTING FAN------------");
	digitalWrite(FAN_OUT, HIGH);
	Serial.println("---------------------------------");
}
void debugLineSensor() {
	Serial.println("---------LINE SENSOR DEBUG-------");
	String line = String(analogRead(LINE_SENSOR_IN));
	Serial.print("LINE SENSOR READING: ");
	Serial.println(line);
	Serial.println("---------------------------------");
}
void changeStepDir() {
	stepDir = !stepDir;
	digitalWrite(STEPPER_DIR, stepDir);
}
void stepMotorTest() {
	for(int i=0;i<10;i++) {
		digitalWrite(STEPPER_STEP, HIGH);
		delay(50);
		digitalWrite(STEPPER_STEP, LOW);
		delay(100);
	}
}
void cycleISR() {
	boolean isButtonPressed = digitalRead(CYCLE_BUTTON_IN);
	if(!isButtonPressed) {
		state++;
		if(state >= 6) {
			state = 0;
		}
	}
}