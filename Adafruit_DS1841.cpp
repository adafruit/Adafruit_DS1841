/*!
 *  @file Adafruit_DS1841.cpp
 *
 *  @mainpage Adafruit DS1841 I2C Logorithmic Resistor
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for the Library for the DS1841 I2C Logorithmic Resistor
 *
 * 	This is a library for the Adafruit DS1841 breakout:
 * 	https://www.adafruit.com/product/4570
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section dependencies Dependencies
 *  This library depends on the Adafruit BusIO library
 *
 *  This library depends on the Adafruit Unified Sensor library
 *
 *  @section author Author
 *
 *  Bryan Siepert for Adafruit Industries
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#include "Arduino.h"
#include <Wire.h>

#include "Adafruit_DS1841.h"

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_DS1841::begin(uint8_t i2c_address, TwoWire *wire) {

  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  return _init();
}

/*!  @brief Initializer for post i2c/spi init
 *   @param sensor_id Optional unique ID for the sensor set
 *   @returns True if chip identified and initialized
 */
bool Adafruit_DS1841::_init(void) {

  enableSaveToEEPROM(false);
  enableUpdateMode(true);
  enableAdderMode(false);
  enableManualLUTAddr(true);
  enableManualWiper(true);

  return true;
}

/**
 * @brief Gets the current wiper value
 *
 * @return uint8_t The current 7-bit wiper value
 */
uint8_t Adafruit_DS1841::getWiper(void) {

  Adafruit_BusIO_Register wiper_register =
      Adafruit_BusIO_Register(i2c_dev, DS1841_WR, 1);
  return wiper_register.read();
}
/**
 * @brief
 *
 * @param wiper_value The 7-bit number to set the wiper value to
 * @return true on success
 * @return false if wiper_value is out of range
 */
bool Adafruit_DS1841::setWiper(uint8_t wiper_value) {
  if ((wiper_value > 127) || (wiper_value < 0)) {
    return false;
  }
  Adafruit_BusIO_Register wiper_register =
      Adafruit_BusIO_Register(i2c_dev, DS1841_WR, 1);

  wiper_register.write(wiper_value);
  return true;
}

/**
 * @brief Gets the current temperature
 *
 * @return int8_t The current temperature in degrees C
 */
int8_t Adafruit_DS1841::getTemperature(void) {

  Adafruit_BusIO_Register temperature_register =
      Adafruit_BusIO_Register(i2c_dev, DS1841_TEMP, 1);

  return (int8_t)temperature_register.read();
}

/**
 * @brief Gets the current temperature
 *
 * @return int8_t The current voltage between VCC and GND in mV
 */
float Adafruit_DS1841::getVoltage(void) {

  Adafruit_BusIO_Register voltage_register =
      Adafruit_BusIO_Register(i2c_dev, DS1841_VOLTAGE, 1);

  return voltage_register.read() * DS1841_VCC_LSB_TO_MA;
}
/**
 * @brief Sets the default Wiper value loaded on boot
 *
 * @param wiper_default The current voltage between VCC and GND in mV
 */
void Adafruit_DS1841::setWiperDefault(uint8_t new_wiper_default) {
  if ((new_wiper_default > 127) || (new_wiper_default < 0)) {
    return false;
  }

  Adafruit_BusIO_Register voltage_register =
      Adafruit_BusIO_Register(i2c_dev, DS1841_VOLTAGE, 1);


        // self._disable_save_to_eeprom = False
        // # allows for IV to pass through to WR.
        // # this setting is also saved to EEPROM so IV will load into WR on boot
        // // self._update_mode = False
        // sleep(0.2)
        // self._initial_value_register = value
        // sleep(0.2)
        // self._disable_save_to_eeprom = True
        // # Turn update mode back on so temp and voltage update
        // # and LUT usage works
        // self._update_mode = True



  return voltage_register.read() * DS1841_VCC_LSB_TO_MA;
}

void Adafruit_DS1841::enableSaveToEEPROM(bool enable_eeprom){

  Adafruit_BusIO_Register config_0 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR0, 1);
  Adafruit_BusIO_RegisterBits disable_eeprom_writes =
    Adafruit_BusIO_RegisterBits(&config_0, 1, 7);

  disable_eeprom_writes.write(!enable_eeprom);

}

//TODO: CHECK POLARITY FOR NAMING
void Adafruit_DS1841::enableUpdateMode(bool enable_update){

  Adafruit_BusIO_Register config_1 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR1, 1);

  Adafruit_BusIO_RegisterBits update_mode =
      Adafruit_BusIO_RegisterBits(&config_1, 1, 0);

  update_mode.write(enable_update);

}

//TODO: CHECK POLARITY FOR NAMING
void Adafruit_DS1841::enableAdderMode(bool enable_adder_mode){

  Adafruit_BusIO_Register config_1 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR1, 1);

  Adafruit_BusIO_RegisterBits adder_mode =
      Adafruit_BusIO_RegisterBits(&config_1, 1, 1);

  adder_mode.write(enable_adder_mode);

}


//TODO: CHECK POLARITY FOR NAMING
void Adafruit_DS1841::enableManualLUTAddr(bool manual_lut_addr){

  Adafruit_BusIO_Register config_2 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR2, 1);
  Adafruit_BusIO_RegisterBits lutar_mode =
      Adafruit_BusIO_RegisterBits(&config_2, 1, 1);

  lutar_mode.write(manual_lut_addr);

}

//TODO: CHECK POLARITY FOR NAMING
void Adafruit_DS1841::enableManualWiper(bool manual_wiper){

  Adafruit_BusIO_Register config_2 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR2, 1);
  Adafruit_BusIO_RegisterBits wiper_access =
      Adafruit_BusIO_RegisterBits(&config_2, 1, 2);

  wiper_access.write(manual_wiper);
}
