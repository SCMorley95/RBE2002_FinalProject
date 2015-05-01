#include <TimerOne.h>

#define STEPPER_STEP 23
#define STEPPER_DIR 24
#define STEPPER_5V_OUT 13
#define FAN_5V_OUT A0
#define FAN_OUT 25

boolean isStepping = 0;
boolean stepperDir = 1;
int steps = 0;

void setup() {
	pinMode(STEPPER_STEP, OUTPUT);
	pinMode(STEPPER_DIR, OUTPUT);
	pinMode(STEPPER_5V_OUT, OUTPUT);
	pinMode(FAN_5V_OUT, OUTPUT);
	digitalWrite(STEPPER_5V_OUT, HIGH);
	Timer1.initialize(100000);
	Timer1.attachInterrupt(turretISR);
	digitalWrite(FAN_5V_OUT, HIGH);
	digitalWrite(FAN_OUT, LOW);
	digitalWrite(12, HIGH);
}
void loop() {
	digitalWrite(STEPPER_DIR, stepperDir);
	digitalWrite(STEPPER_STEP, isStepping);
	if(isStepping) {
		isStepping = 0;
	}
}
void turretISR() {
	isStepping = 1;
	steps++;
	if(steps >= 70) {
		stepperDir = !stepperDir;
		steps = 0;
	}
}