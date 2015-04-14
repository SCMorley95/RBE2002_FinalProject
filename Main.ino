#include <QueueList.h> //download this!!
#include <StepperMotor.h>
#include <Serial.h>

#define TURRET_DIR_FORWARDS true;
#define TURRET_DIR_BACKWARDS false;

boolean isTurretRotation = true;
boolean turretDirection = TURRET_DIR_FORWARDS;

void setup() {
	StepperMotor turretMotor = new StepperMotor(STEP_PIN, DIR_PIN);
	//instantiate Encoder module
	//Gyro module
	//PID module
	//Wheel controller module
	//Turret controller module
	//Flame sensor module
	//IR module
	//Ultrasonic module
	//Display module
}
void loop() {
	state = setObjective();
	switch(state) {
		case default:
			break;
		case 0: //about to go off cliff
			stopRobot();
			moveFwd(-10);
			turnDeg(-90);
			break;
		case 1: //flame detected
			stopRobot();
			isTurretRotation = false;
			while(!flameOut()) {
				fanMotor.write(FAN_FULL_ON);
			}
			reportLocation();
			break;
		case 2: //walls on front and right
			turnDeg(-90);
			break;
		case 3: //wall just on right
			setRobotSpeed(ROBOT_FULL_FORWARD);
			break;
		case 4: //wall just in front
			turnDeg(90);
			break;
		case 5: //no walls detected
			turnDeg(90);
			break;
		//etc
	}
}
int setObjective() {
	if(!checkLine()) {
		return 0;
	}
	if(isFlame()) {
		return 1;
	}
	else if(wallSide()) {
		if(wallFront()) {
			return 2;
		}
		else {
			return 3;
		}
	}
	else {
		if(wallFront()) {
			return 4;
		}
		else {
			return 5;
		}
	}
}
boolean checkLine() {
	//check the line sensor to detect if we are over a black line ( black line == cliff == SCARY!!!!!)
}
boolean wallFront() {
	//check the ultrasonic sensor to see if the detected distance is under (insert appropriate comparator here)
}
boolean wallSide() {
	//check the LIDAR to see if (something something LIDAR something something wall on the right)
}
boolean isFlame() {
	//look real hard for a flame
}
boolean flameOut() {
	//look slightly less hard for a flame (or if that ends up not working just slap a timer up in this)
}
void turretISR() {
	if(isTurretRotation) {
		turretMotor.step(turretDirection);
		if(turretMotor.getPos() = 180) {
			turretDirection = TURRET_DIR_BACKWARDS;
		}
		else if(turretMotor.getPos() = 0) {
			turretDirection = TURRET_DIR_FORWARDS;
		}
	}
}