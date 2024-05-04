/*******************************************************************************
 * Copyright (c) 2021 Renesas Electronics Corporation
 * All Rights Reserved.
 *
 * This code is proprietary to Renesas, and is license pursuant to the terms and
 * conditions that may be accessed at:
 * https://www.renesas.com/eu/en/document/msc/renesas-software-license-terms-gas-sensor-software
 *
 ******************************************************************************/

/**
 * @file    i2c_wrapper_araduino.h
 * @brief   I2C wrapper functions for Arduino
 * @version 2.4.2
 * @author Renesas Electronics Corporation
 */

#ifndef _I2C_WRAPPER_ARDUINO_
#define _I2C_WRAPPER_ARDUINO_

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <zmod4xxx_hal.h>

/**
 * @brief Initialize the I2C communication for Arduino
 */
void arduino_i2c_setup();

/**
 * @brief Sleep for some time. Depending on target and application this can \n
 *        be used to go into power down or to do task switching.
 * @param [in] ms will sleep for at least this number of milliseconds
 */
void arduino_delay(uint32_t ms);

/* I2C communication */
/**
 * @brief Read a register(s) over I2C
 * @param [in] i2c_addr 7-bit I2C slave address of the ZMOD44xx
 * @param [in] reg_addr address of internal register to read
 * @param [out] buf destination buffer; must have at least a size of len
 * @param [in] len number of bytes to read
 * @return error code
 */
int8_t arduino_i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buf,
                        uint8_t len);

/**
 * @brief Write a register(s) over I2C
 * @param [in] i2c_addr 7-bit I2C slave address of the ZMOD4xxx
 * @param [in] reg_addr address of internal register to write
 * @param [in] buf source buffer; must have at least a size of len
 * @param [in] len number of bytes to write
 * @return error code
 */
int8_t arduino_i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buf,
                         uint8_t len);

#endif //_I2C_WRAPPER_ARDUINO_
