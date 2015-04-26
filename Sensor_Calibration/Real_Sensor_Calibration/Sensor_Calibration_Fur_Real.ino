#include <Encoder.h>
#include <ultrasonic.h>
#include <l3G.h>

#define US_PING 24
#define US_ECHO 25
#define FLAME_IN 2
#define LINE_SENSOR_IN 3
#define ENCODER_IN_LEFT 28
#define ENCODER_IN_RIGHT 29
#define LIDAR_IN 30

double Setpoint, Input, Output;
Encoder encLeft (ENCODER_IN_LEFT);
Encoder encRight (ENCODER_IN_RIGHT);
ultrasonic myUltraSonic (US_PING, US_ECHO);
PID gyroPID(&Input, &Output, &Setpoint, PVAL, IVAL, DVAL, DIRECT);
L3G gyro;

void setup() {
	Serial.begin(9600);
	Wire.begin();
	if(!gyro.init()) {
		Serial.println("Well that didnt work");
	}
	gyro.enableDefault();
	gyroPID.SetOutputLimits(0,30);
	pinMode(LIDAR_IN, INPUT);
	pinMode(FLAME_IN, INPUT);
	pinMode(LINE_SENSOR_IN, INPUT);
}
void loop() {
	gyro.read();
	int LIDAR_RAW = analogRead(LIDAR_IN);
	int LINE_SENSOR_RAW = analogRead(LINE_SENSOR_IN);
	int FLAME_RAW = analogRead(FLAME_IN);
	int LEFT_ENCODER = encLeft.read();
	int RIGHT_ENCODER = encRight.read();
	float GYRO_Z = gyro.g.z;
	Serial.printf("Lidar reading: %3d \n", LIDAR_RAW);
	Serial.printf("Line Sensor reading: %3d \n", LINE_SENSOR_RAW);
	Serial.printf("Flame Sensor Reading: %3d \n", FLAME_RAW);
	Serial.printf("Left Encoder Reading: %3d \n", LEFT_ENCODER);
	Serial.printf("Right Encoder Reading: %3d \n", RIGHT_ENCODER);
	Serial.printf("Gyro Z Axis Reading: %3d \n", GYRO_Z);
	Serial.println("----------------------------------------");
	delay(100);
}