#include <Servo.h>
#include <Encoder.h>

#define ENCODER_IN_LEFT_1 2
#define ENCODER_IN_LEFT_2 3
#define ENCODER_IN_RIGHT_1 18
#define ENCODER_IN_RIGHT_2 19
#define SERVO_LEFT 10
#define SERVO_RIGHT 11

Servo leftServo;
Servo rightServo;
Encoder encLeft (ENCODER_IN_LEFT_1, ENCODER_IN_LEFT_2);
Encoder encRight (ENCODER_IN_RIGHT_1, ENCODER_IN_RIGHT_2);

void setup() {
	leftServo.attach(SERVO_LEFT);
	rightServo.attach(SERVO_RIGHT);
	Serial.begin(9600);
}
void loop() {
	int lEnc = encLeft.read();
	int rEnc = encRight.read();
	Serial.print(String(lEnc));
	Serial.print(" ");
	Serial.println(String(rEnc));
	leftServo.write(100);
	rightServo.write(110);
}