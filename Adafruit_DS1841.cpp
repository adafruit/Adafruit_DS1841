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
  enableAdderMode(false);
  enableManualLUTAddr(true);
  enableManualWiper(true);
  enableUpdateMode(true);

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
  if (wiper_value > 127) {
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
 * @param new_wiper_default The value to set the wiper to
 * @return true The update was successful
 * @return false The new value was out of range
 */
bool Adafruit_DS1841::setWiperDefault(uint8_t new_wiper_default) {
  if (new_wiper_default > 127) {
    return false;
  }

  Adafruit_BusIO_Register initial_value =
      Adafruit_BusIO_Register(i2c_dev, DS1841_IVR, 1);

  // self._disable_save_to_eeprom = False
  enableSaveToEEPROM(true);

  // # allows for IV to pass through to WR.
  // # this setting is also saved to EEPROM so IV will load into WR on boot
  // // self._update_mode = False
  enableUpdateMode(false);
  delay(20);

  // self._initial_value_register = value
  initial_value.write(new_wiper_default);
  delay(20);

  // self._disable_save_to_eeprom = True
  enableSaveToEEPROM(false);

  // # Turn update mode back on so temp and voltage update
  // # and LUT usage works
  enableUpdateMode(true);

  return true;
}

/**
 * @brief Determines if the values set in IV and CR1 are saved to EEPROM
 *
 * @param enable_eeprom True enables shadowing IV and CR1 values to EEPROM
 */
void Adafruit_DS1841::enableSaveToEEPROM(bool enable_eeprom) {

  Adafruit_BusIO_Register config_0 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR0, 1);
  Adafruit_BusIO_RegisterBits disable_eeprom_writes =
      Adafruit_BusIO_RegisterBits(&config_0, 1, 7);

  disable_eeprom_writes.write(!enable_eeprom);
}

/**
 * @brief Determines if WR, Temperature, and Voltage measurements are updated
 *
 * @param enable_update If True, Temperature and Voltage are updated regularly
 * and WR can be set manually or using the LUT
 */
void Adafruit_DS1841::enableUpdateMode(bool enable_update) {

  Adafruit_BusIO_Register config_1 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR1, 1);

  Adafruit_BusIO_RegisterBits update_mode =
      Adafruit_BusIO_RegisterBits(&config_1, 1, 0);

  update_mode.write(enable_update);
}

/**
 * @brief Determines if the Value from the LUT is summed with or used directly
 *
 * @param enable_adder_mode If true, the initial value is added to the value
 * from the LUT, acting as an offset. If false, the initial value and LUT value
 * are used directly
 */
void Adafruit_DS1841::enableAdderMode(bool enable_adder_mode) {

  Adafruit_BusIO_Register config_1 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR1, 1);

  Adafruit_BusIO_RegisterBits adder_mode =
      Adafruit_BusIO_RegisterBits(&config_1, 1, 1);

  adder_mode.write(enable_adder_mode);
}

/**
 * @brief Determines how the index into the LUT is determined
 *
 * @param manual_lut_addr If true, the LUT address is set manually. If false,
 * the LUT address is selected automatically based on the temperature. See the
 * datasheet for details.
 */
void Adafruit_DS1841::enableManualLUTAddr(bool manual_lut_addr) {

  Adafruit_BusIO_Register config_2 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR2, 1);
  Adafruit_BusIO_RegisterBits lutar_mode =
      Adafruit_BusIO_RegisterBits(&config_2, 1, 1);

  lutar_mode.write(manual_lut_addr);
}

/**
 * @brief Determines how the wiper value is set.
 *
 * @param manual_wiper If true, the wiper value is only updated manually. If
 * false, the wiper value is determined by the LUT
 */
void Adafruit_DS1841::enableManualWiper(bool manual_wiper) {

  Adafruit_BusIO_Register config_2 =
      Adafruit_BusIO_Register(i2c_dev, DS1841_CR2, 1);
  Adafruit_BusIO_RegisterBits wiper_access =
      Adafruit_BusIO_RegisterBits(&config_2, 1, 2);

  wiper_access.write(manual_wiper);
}

/**
 * @brief Puts the DS1841 into a confituration that allows for manually updating
 * and using the LUT
 *
 * @param enable_lut_mode true disables manually setting the wiper, and enabled
 * manualy setting the index into the LUT. false disables manually setting the
 * LUT index and enables setting the wiper manually.
 */
void Adafruit_DS1841::enableLUTMode(bool enable_lut_mode) {
  enableManualLUTAddr(enable_lut_mode);
  enableUpdateMode(true);
  enableManualWiper(!enable_lut_mode);
}

/**
 * @brief Gets the currently selected offset into the LUT
 *
 * @return uint8_t The index of the selected LUT entry
 */
uint8_t Adafruit_DS1841::getLUTSelection(void) {

  Adafruit_BusIO_Register lut_selection =
      Adafruit_BusIO_Register(i2c_dev, DS1841_LUTAR, 1);
  return lut_selection.read() - DS1841_LUT;
}

/**
 * @brief
 *
 * @param lut_index The 0-based offset into the LUT to select for use
 * @return true on success
 * @return false if lut_index is out of range
 */

bool Adafruit_DS1841::setLUTSelection(uint8_t lut_index) {
  if (lut_index > 71) {
    return false;
  }
  Adafruit_BusIO_Register lut_selection =
      Adafruit_BusIO_Register(i2c_dev, DS1841_LUTAR, 1);

  lut_selection.write(lut_index + DS1841_LUT);
  return true;
}

/**
 * @brief Gets the value for the LUT entry at the given index
 * @param index The 0-based offset into the LUT to retrieve
 *
 * @return uint8_t The current 7-bit wiper value
 */
uint8_t Adafruit_DS1841::getLUT(uint8_t index) {

  Adafruit_BusIO_Register lut_entry =
      Adafruit_BusIO_Register(i2c_dev, DS1841_LUT + index, 1);
  return lut_entry.read();
}

/**
 * @brief
 *
 * @param index The 0-based offset into the LUT to retrieve
 * @param lut_value The 7-bit number to set LUT entry to
 * @return true on success
 * @return false if lut_value is out of range
 */
bool Adafruit_DS1841::setLUT(uint8_t index, uint8_t lut_value) {
  if (lut_value > 127) {
    return false;
  }
  Adafruit_BusIO_Register lut_entry =
      Adafruit_BusIO_Register(i2c_dev, DS1841_LUT + index, 1);

  lut_entry.write(lut_value);
  delay(20);
  return true;
}
