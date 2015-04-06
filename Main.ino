#include <QueueList.h> //download this!!
//values for all of the cases
//#define ... 0
QueueList <int> NormalQueue;
QueueList <int> PriorityQueue;
void setup() {
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
	setObjective();
	if(!PriorityQueue.isEmpty()) {
		state = PriorityQueue.pop();
	}
	else if(!PriorityQueue.isEmpty()) {
		state = NormalQueue.pop();
	}
	else {
		state = default;
	}
	switch(state) {
		case default:
			break;
		case 0:
		case 1:
		case 2:
		//etc
	}
}
void setObjective() {

}