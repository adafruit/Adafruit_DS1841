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
  uint8_t temp_i = 0;
  float temp_f = 0.0;

  Serial.println("DS1841 Found!");
  pinMode(VOLTAGE_DIV_PIN, INPUT);

  Serial.println("Initializing wiper to 0x72");
  ds.setWiper(0x72);
  temp_i = ds.getWiper();
  Serial.print("Wiper is: 0x");Serial.println(temp_i, HEX);

  temp_i = ds.getLUT(0x42);
  Serial.print("Lut 0x42: 0x");Serial.println(temp_i, HEX);

  Serial.println("Setting LUT 0x42 to 0x21");
  ds.setLUT(0x42, 0x21);
  temp_i = ds.getLUT(0x42);
  Serial.print("After: Lut 0x42: 0x");Serial.println(temp_i, HEX);


  temp_i = ds.getWiper();
  Serial.print("Wiper: 0x");Serial.println(temp_i, HEX);

  Serial.println("Enabling LUT mode");
  ds.enableLUTMode(true);

  temp_i = ds.getWiper();
  Serial.print("Wiper now: 0x");Serial.println(temp_i, HEX);

  temp_i = ds.getLUTSelection();
  Serial.print("Lut selection: 0x");Serial.println(temp_i, HEX);
  Serial.println("Setting to 0x42");
  ds.setLUTSelection(0x42);

  temp_i = ds.getLUTSelection();
  Serial.print("New Lut selection: 0x");Serial.println(temp_i, HEX);

  temp_i = ds.getWiper();
  Serial.print("Wiper now: 0x");Serial.println(temp_i, HEX);

  Serial.println("Disabling LUT mode");
  ds.enableLUTMode(false);

  Serial.println("Setting wiper:");
  ds.setWiper(0x66);

  temp_i = ds.getWiper();
  Serial.print("Wiper now: 0x");Serial.println(temp_i, HEX);

}

void loop() {
  delay(1000);
}
