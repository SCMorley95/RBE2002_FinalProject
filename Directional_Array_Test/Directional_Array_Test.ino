#include <directionalArray.h>

directionalArray dirArray(0,1); //start DirArray with x value of 0 and y value of 1 (facing upwards)

void setup() {
	Serial.begin(9600);
	Serial.println("Beginning dirArray test, initial conditions: [0,1]");
}
void loop() {
	delay(1000);
	Serial.println("Rotating array 90 degrees (counterclockwise?), expected new value [-1, 0]");
	dirArray.rotate(90);
	int dirX = dirArray.xval();
	int dirY = dirArray.yval();
	Serial.print("X: ");
	Serial.print(dirX);
	Serial.print("  ");
	Serial.print("Y: ");
	Serial.print(dirY);
	while(1);
}