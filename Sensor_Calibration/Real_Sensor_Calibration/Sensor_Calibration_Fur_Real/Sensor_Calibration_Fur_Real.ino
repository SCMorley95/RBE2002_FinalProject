#include <Encoder.h>
#include <ultrasonic.h>
#include <PID_v1.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define US_PING 28
#define US_ECHO 29
#define US_PING_1 26
#define US_ECHO_1 27
#define FLAME_IN 10
#define LINE_SENSOR_IN 11
#define ENCODER_IN_LEFT 2
#define ENCODER_IN_LEFT_1 3
#define ENCODER_IN_RIGHT 18
#define ENCODER_IN_RIGHT_1 19
#define BUTTON_IN 22
#define V_OUT_LCD 12
#define V_OUT_STEPPER 13
#define STEPPER_STEP 23
#define STEPPER_DIR 24

#define PVAL 5
#define IVAL 1
#define DVAL 2

double Setpoint, Input, Output;
Encoder encLeft (ENCODER_IN_LEFT, ENCODER_IN_LEFT_1);
Encoder encRight (ENCODER_IN_RIGHT, ENCODER_IN_RIGHT_1);
ultrasonic ultraF (US_PING, US_ECHO);
ultrasonic ultraR (US_PING_1, US_ECHO_1);
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
boolean isStep;

void setup() {
	lcd.begin(16,2);
	/*
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	*/
	pinMode(V_OUT_LCD, OUTPUT);
	pinMode(V_OUT_STEPPER, OUTPUT);
	pinMode(BUTTON_IN, INPUT_PULLUP);
	pinMode(STEPPER_STEP, OUTPUT);
	pinMode(STEPPER_DIR, OUTPUT);
	Serial.begin(9600);
	Serial.println("Made it here!");
	Wire.begin();
	Serial.println("Initialized gyro");
	pinMode(FLAME_IN, INPUT);
	pinMode(LINE_SENSOR_IN, INPUT);
	Serial.println("Done with setup");
	lcd.print("Done with setup");
	while(digitalRead(BUTTON_IN));
	lcd.setCursor(0,1);
	lcd.print("Stepper Debug");
	digitalWrite(STEPPER_DIR, LOW);
}
void loop() {
	isStep = !digitalRead(BUTTON_IN);
	digitalWrite(V_OUT_LCD, HIGH);
	digitalWrite(V_OUT_STEPPER, HIGH);
	digitalWrite(STEPPER_STEP, isStep);
	digitalWrite(STEPPER_DIR, HIGH);
	int LINE_SENSOR_RAW = analogRead(LINE_SENSOR_IN);
	int FLAME_RAW = analogRead(FLAME_IN);
	int LEFT_ENCODER = encLeft.read();
	int RIGHT_ENCODER = encRight.read();
	//Serial.println(String(LINE_SENSOR_RAW));
	//Serial.println(String(FLAME_RAW));
	//Serial.println(String(LEFT_ENCODER));
	//Serial.println(String(RIGHT_ENCODER));
	//Serial.println(String(GYRO_Z));
	/*
	Serial.printf("Lidar reading: %3d \n", LIDAR_RAW);
	Serial.printf("Line Sensor reading: %3d \n", LINE_SENSOR_RAW);
	Serial.printf("Flame Sensor Reading: %3d \n", FLAME_RAW);
	Serial.printf("Left Encoder Reading: %3d \n", LEFT_ENCODER);
	Serial.printf("Right Encoder Reading: %3d \n", RIGHT_ENCODER);
	Serial.printf("Gyro Z Axis Reading: %3d \n", GYRO_Z);
	*/
	Serial.println("----------------------------------------");
}
