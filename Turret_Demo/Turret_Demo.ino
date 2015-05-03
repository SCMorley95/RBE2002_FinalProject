#include <TimerOne.h>
#include <Encoder.h>

#define STEPPER_STEP 23
#define STEPPER_DIR 24
#define STEPPER_5V_OUT A1
#define FAN_5V_OUT A0
#define LCD_5V_OUT A2
#define FAN_OUT 25
#define BUTTON_IN 22
#define FLAME_IN A4
#define ENCODER_IN_LEFT_1 2
#define ENCODER_IN_LEFT_2 3
#define ENCODER_IN_RIGHT_1 18
#define ENCODER_IN_RIGHT_2 19
#define STEPPER_ENABLE 12
boolean stepperDir = 1;
int steps = 0;
int fireSensor;
Encoder leftEncoder(ENCODER_IN_LEFT_2, ENCODER_IN_LEFT_2);
Encoder rightEncoder(ENCODER_IN_RIGHT_1, ENCODER_IN_RIGHT_2);

void setup() {
	Serial.begin(9600);
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
}
void loop() {
	int leftIn = leftEncoder.read();
	int rightIn = rightEncoder.read();
	/*
	Serial.print("Left Encoder:  ");
	Serial.println(leftIn);
	Serial.print("Right Encoder:  :");
	Serial.println(rightIn);
	*/
	if(fireSensor < 300 && fireSensor != 0) {
		noInterrupts();
		digitalWrite(STEPPER_STEP, LOW);
		digitalWrite(FAN_OUT, HIGH);
		digitalWrite(STEPPER_ENABLE, HIGH);
		while(1) {
			if(!digitalRead(BUTTON_IN)) {
				digitalWrite(FAN_OUT, LOW);
				while(1);
			}
		}
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