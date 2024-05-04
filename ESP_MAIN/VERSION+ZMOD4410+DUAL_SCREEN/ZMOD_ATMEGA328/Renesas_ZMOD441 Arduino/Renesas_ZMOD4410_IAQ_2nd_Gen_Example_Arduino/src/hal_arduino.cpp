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
 * @file    hal_arduino.cpp
 * @brief   zmod4xxx hardware abstraction layer for Arduino platform
 * @version 2.4.2
 * @author Renesas Electronics Corporation
 */

#include "hal_arduino.h"

zmod4xxx_err init_hardware(zmod4xxx_dev_t *dev)
{
    arduino_i2c_setup();
    dev->read = arduino_i2c_read;
    dev->write = arduino_i2c_write;
    dev->delay_ms = arduino_delay;
    return ZMOD4XXX_OK;
}

zmod4xxx_err deinit_hardware()
{
    return ZMOD4XXX_OK;
}
