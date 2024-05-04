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
 * @file    i2c_wrapper_araduino.cpp
 * @brief   I2C wrapper functions for Arduino
 * @version 2.4.2
 * @author Renesas Electronics Corporation
 */

#include "i2c_wrapper_arduino.h"

void arduino_i2c_setup()
{
    Wire.begin();
}

void arduino_delay(uint32_t ms)
{
    delay(ms);
}

int8_t arduino_i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buf,
                        uint8_t len)
{
    Wire.beginTransmission(i2c_addr);
    Wire.write(reg_addr);
    Wire.endTransmission(false);
    delay(10);
    Wire.requestFrom(i2c_addr, len);

    while (Wire.available()) // slave may send less than requested
    {
        for (int i = 0; i < len; i++) {
            buf[i] = Wire.read();
        }
    }
    Wire.endTransmission();
    return 0;
}

int8_t arduino_i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buf,
                         uint8_t len)
{
    Wire.beginTransmission(i2c_addr);
    Wire.write(reg_addr);
    Wire.write(buf, len);
    Wire.endTransmission();
    return 0;
}
