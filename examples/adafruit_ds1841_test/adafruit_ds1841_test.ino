// Basic demo for readings from Adafruit DS1841
#include <Wire.h>
#include <Adafruit_DS1841.h>
#define VOLTAGE_DIV_PIN A0
float wiperVoltage(void);
float wiper_voltage;

Adafruit_DS1841  ds;
void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit DS1841 test!");

  // Try to initialize!
  if (!ds.begin()) {
    Serial.println("Failed to find DS1841 chip");
    while (1) { delay(10); }
  }
  Serial.println("DS1841 Found!");
  Serial.print("Current VCC Voltage:"); Serial.print(ds.getVoltage()); Serial.println("mV");
  Serial.print("Temperature: ");Serial.print(ds.getTemperature());Serial.println(" degrees C");

  pinMode(VOLTAGE_DIV_PIN, INPUT);

}

void loop() {
  ds.setWiper(10);
  delay(1000);
  wiper_voltage = wiperVoltage();
  Serial.print("Voltage:");Serial.println(wiper_voltage);
  Serial.print("Temperature: ");Serial.print(ds.getTemperature());Serial.println(" degrees C");
  Serial.print("Wiper: ");Serial.print(ds.getWiper());Serial.println(" LSB");
  Serial.println("");
  
  ds.setWiper(120);
  delay(1000);
  wiper_voltage = wiperVoltage();
  Serial.print("Voltage:");Serial.println(wiper_voltage);
  Serial.print("Temperature: ");Serial.print(ds.getTemperature());Serial.println(" degrees C");
  Serial.print("Wiper: ");Serial.print(ds.getWiper());Serial.println(" LSB");
  Serial.println("");
}

float wiperVoltage(void) {
  float wiper_value = analogRead(VOLTAGE_DIV_PIN);
  wiper_value *= 5.0;
  wiper_value /= 1024;
  return wiper_value;
}