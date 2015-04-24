#include <ultrasonic.h>
ultrasonic myUS (24, 25);
void setup() {
	
}
void loop() {
	float distFloat = myUS.distance();
	String dist = String(distFloat);
	Serial.println(dist);
}