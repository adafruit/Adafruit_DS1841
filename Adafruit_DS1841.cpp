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
 * 	https://www.adafruit.com/product/45XX
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
 *    @param  sensor_id
 *            The unique ID to differentiate the sensors from others
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_DS1841::begin_I2C(uint8_t i2c_address, TwoWire *wire) {

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

  // make sure we're talking to the right chip
  // still need to find a way to verify

  // self._eeprom_shadow_bit = True # turn off eeprom updates to IV

  Adafruit_BusIO_Register config_0 = Adafruit_BusIO_Register(i2c_dev, DS1841_CR0, 1);
  Adafruit_BusIO_RegisterBits eeprom_shadow = Adafruit_BusIO_RegisterBits(&config_0, 1, 7);

  Adafruit_BusIO_Register config_1 = Adafruit_BusIO_Register(i2c_dev, DS1841_CR1, 1);
  Adafruit_BusIO_RegisterBits update_mode = Adafruit_BusIO_RegisterBits(&config_1, 1, 0);
  Adafruit_BusIO_RegisterBits adder_mode = Adafruit_BusIO_RegisterBits(&config_1, 1, 1);

  Adafruit_BusIO_Register config_2 = Adafruit_BusIO_Register(i2c_dev, DS1841_CR2, 1);
  Adafruit_BusIO_RegisterBits lutar_mode = Adafruit_BusIO_RegisterBits(&config_2, 1, 1);
  Adafruit_BusIO_RegisterBits wiper_access = Adafruit_BusIO_RegisterBits(&config_2, 1, 2);

  eeprom_shadow.write(1);
  update_mode.write(1);
  adder_mode.write(0);

  lutar_mode.write(1);
  wiper_access.write(1);

  return true;
}

/**
 * @brief Gets the current wiper value
 *
 * @return uint8_t The current 7-bit wiper value
 */
uint8_t Adafruit_DS1841::getWiper(void) {

  Adafruit_BusIO_Register wiper_register = Adafruit_BusIO_Register(
      i2c_dev, DS1841_WR, 1);
  return wiper_register.read();
}
/**
 * @brief Sets the rate at which pressure and temperature measurements
 *
 * @param wiper_value The 7-bit number to set the wiper value to
 */
bool Adafruit_DS1841::setWiper(uint8_t wiper_value) {
  if (wiper_value > 127){
    return false;
  }
  Adafruit_BusIO_Register wiper_register = Adafruit_BusIO_Register(
      i2c_dev, DS1841_WR, 1);

  wiper_register.write(wiper_value);
  return true;
}

/**
 * @brief Gets the current temperature
 *
 * @return int8_t The current temperature in degrees C
 */
int8_t Adafruit_DS1841::getTemperature(void) {

  Adafruit_BusIO_Register temperature_register = Adafruit_BusIO_Register(
      i2c_dev, DS1841_TEMP, 1);

  return (int8_t)temperature_register.read();
}