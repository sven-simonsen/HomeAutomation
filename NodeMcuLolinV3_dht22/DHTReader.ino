#include <DHT.h>
#define DHT_TYPE DHT22


#define DHT_PIN D1

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  // wait for serial to initialise
  while(!Serial){}
}

// runs over and over again
void loop() {
	float h = dht.readHumidity();
	float t = dht.readTemperature();
	
	Serial.print("Temperatur: ");
	Serial.print(t);
	Serial.print("°C, Luftfeuchtigkeit: ");
	Serial.print(h);
	Serial.println("%");
	
	delay(2000);
}