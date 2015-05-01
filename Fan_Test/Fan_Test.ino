void setup() {
	pinMode(25, OUTPUT);
}
void loop() {
	delay(500);
	digitalWrite(25, HIGH);
	delay(500);
	digitalWrite(25, LOW);
}