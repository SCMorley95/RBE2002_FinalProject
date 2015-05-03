#include <TimerOne.h>

#define STEPPER_STEP 23
#define STEPPER_DIR 24
#define STEPPER_5V_OUT A1
#define FAN_5V_OUT A0
#define LCD_5V_Out A2
#define FAN_OUT 25
#define BUTTON_IN 22
#define FLAME_IN A4
boolean isStepping = 0;
boolean stepperDir = 1;
boolean isFanOn = 0;
int steps = 0;
int fireSensor;

void setup() {
	pinMode(STEPPER_STEP, OUTPUT);
	pinMode(STEPPER_DIR, OUTPUT);
	pinMode(STEPPER_5V_OUT, OUTPUT);
	pinMode(FAN_5V_OUT, OUTPUT);
	pinMode(BUTTON_IN, INPUT_PULLUP);
	pinMode(FLAME_IN, INPUT);
	digitalWrite(STEPPER_5V_OUT, HIGH);
	Timer1.initialize(100000);
	Timer1.attachInterrupt(turretISR);
	digitalWrite(FAN_5V_OUT, HIGH);
	digitalWrite(FAN_OUT, LOW);
	digitalWrite(12, HIGH);
}
void loop() {
	Serial.println(fireSensor);
	if(isStepping) {
		isStepping = 0;
	}
	if(!digitalRead(BUTTON_IN)) {
		isFanOn = !isFanOn;
	}
	digitalWrite(STEPPER_DIR, stepperDir);
	digitalWrite(STEPPER_STEP, isStepping);
	digitalWrite(FAN_OUT, isFanOn);
}
void turretISR() {
	//fireSensor = analogRead(FLAME_IN);
	isStepping = 1;
	steps++;
	if(steps >= 50) {
		stepperDir = !stepperDir;
		steps = 0;
	}
}