#include "DHT.h" //adafruit library

#define DHT22_PIN 5
#define DHTTYPE DHT22

#define PWM_PIN 3
DHT dht(DHT22_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(DHT22_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  dht.begin();
}

void loop() {

  float h = dht.readHumidity();

  analogWrite(PWM_PIN, map(h, 0, 100, 225, 25));
  Serial.print(map(h, 0, 100, 225, 25));
  Serial.print(" | ");
  Serial.print(h);
  Serial.println();
  delay(1000);
}
