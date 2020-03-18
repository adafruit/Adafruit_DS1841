/*!
 *  @file Adafruit_DS1841.h
 *
 * 	I2C Driver for the Adafruit DS1841 I2C Logorithmic Resistor
 *
 * 	This is a library for the Adafruit DS1841 breakout:
 * 	https://www.adafruit.com/products/45XX
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_DS1841_H
#define _ADAFRUIT_DS1841_H

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>

#define DS1841_IVR 0x00
#define DS1841_CR0 0x02
#define DS1841_CR1 0x03
#define DS1841_LUTAR 0x08
#define DS1841_WR 0x09
#define DS1841_CR2 0x0A
#define DS1841_TEMP 0x0C
#define DS1841_VOLTAGE 0x0E
// DS1841_LUT 0x80–C7h

#define DS1841_VCC_LSB 25.6
#define DS1841_I2CADDR_DEFAULT 0x28 // up to 0x2B

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the DS1841 I2C Logorithmic Resistor
 */
class Adafruit_DS1841 {
public:

  bool begin_I2C(uint8_t i2c_addr = DS1841_I2CADDR_DEFAULT,
                 TwoWire *wire = &Wire);

  int8_t getTemperature(void);
  uint8_t getWiper(void);
  bool setWiper(uint8_t new_wiper_value);
  void setWiperDefault(uint8_t new_wiper_default);

private:
  bool _init(void);
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface

};

#endif