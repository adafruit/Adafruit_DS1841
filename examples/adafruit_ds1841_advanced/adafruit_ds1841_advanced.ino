// Basic demo for readings from Adafruit DS1841
#include <Wire.h>
#include <Adafruit_DS1841.h>

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
  ds.setWiper(0x22);
  Serial.print("Wiper: ");Serial.print(ds.getWiper());Serial.println(" LSB");

  // Run once with this to set the default, then once with it commented out.
  ds.setWiperDefault(0x11);
  Serial.print("Wiper: ");Serial.print(ds.getWiper());Serial.println(" LSB");

}

void loop() {
  ds.setWiper(10);
  delay(1000);
  Serial.print("Wiper: ");Serial.print(ds.getWiper());Serial.println(" LSB");
  ds.setWiper(120);
  delay(1000);
  Serial.print("Wiper: ");Serial.print(ds.getWiper());Serial.println(" LSB");
  Serial.println("");
}