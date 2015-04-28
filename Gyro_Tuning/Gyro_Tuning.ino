#include <ultrasonic.h>

#define US_1_1 28
#define US_1_2 29
#define US_2_1 26
#define US_2_2 27

ultrasonic ultra1(US_1_1, US_1_2);
ultrasonic ultra2(US_2_1, US_2_2);

void setup() {
	Serial.begin(9600);
}
void loop() {
	int dist1 = ultra1.distance();
	int dist2 = ultra2.distance();
	Serial.print(String(dist1));
	Serial.print(" ");
	Serial.println(String(dist2));
}