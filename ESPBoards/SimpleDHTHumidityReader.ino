#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
uint8_t DHTPin = D2;
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(9600);
  delay(2000);
  
  pinMode(DHTPin, INPUT);

  dht.begin();
}

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
