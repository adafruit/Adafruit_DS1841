// Basic demo for accelerometer readings from Adafruit DS1841

Adafruit_DS1841 ds;


}



// Basic demo for readings from Adafruit DS1841
#include <Wire.h>
#include <Adafruit_DS1841.h>

// For SPI mode, we need a CS pin
#define DS_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define DS_SCK 13
#define DS_MISO 12
#define DS_MOSI 11

Adafruit_DS1841  ds; // TODO FIX NAME
void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit DS1841 test!");

  // Try to initialize!
  if (!ds.begin_I2C()) {
  //if (!ds.begin_SPI(DS_CS)) {
  //if (!ds.begin_SPI(DS_CS, DS_SCK, DS_MISO, DS_MOSI)) {
    Serial.println("Failed to find DS1841 chip");
    while (1) { delay(10); }
  }
  Serial.println("DS1841 Found!");

//  ds.setDataRate(DS1841_RATE_12_5_HZ);
  Serial.print("Data rate set to: ");
  switch (ds.getDataRate()) {
    case DS1841_RATE_ONE_SHOT: Serial.println("One Shot"); break;
    case DS1841_RATE_1_HZ: Serial.println("1 Hz"); break;
    case DS1841_RATE_7_HZ: Serial.println("7 Hz"); break;

  }

void loop() {

  sensors_event_t temp;
  sensors_event_t pressure;
  ds.getEvent(&pressure, &temp);// get pressure
  Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print("Pressure: ");Serial.print(pressure.pressure);Serial.println(" hPa");
  Serial.println("");
  delay(100);
}
 sensors_event_t temp;
  sensors_event_t pressure;
  ds.getEvent(&pressure, &temp);// get pressure
  Serial.print(" Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print(" Pressure: ");Serial.print(pressure.pressure);Serial.println(" hPa");
  Serial.println("");
  delay(100);
}
