#include <Arduino.h>
#include <Servo.h>
#include <PID.h>
class WheelController {
public:
	WheelController(int Lpin, int Rpin);
	void goForwards();
	void adjustHeading(int deg);
	void turn(int deg);
	void halt();
	int Lval;
	int Rval;
}