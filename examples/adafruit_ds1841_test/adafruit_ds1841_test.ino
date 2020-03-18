// Basic demo for readings from Adafruit DS1841
#include <Wire.h>
#include <Adafruit_DS1841.h>

Adafruit_DS1841  ds;
void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit DS1841 test!");

  // Try to initialize!
  if (!ds.begin_I2C()) {
    Serial.println("Failed to find DS1841 chip");
    while (1) { delay(10); }
  }
  Serial.println("DS1841 Found!");

}

void loop() {

  Serial.print("Temperature: ");Serial.print(ds.getTemperature());Serial.println(" degrees C");
  Serial.println("");
  delay(100);
}