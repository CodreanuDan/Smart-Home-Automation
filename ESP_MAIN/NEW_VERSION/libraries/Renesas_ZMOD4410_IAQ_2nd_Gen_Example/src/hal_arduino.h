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
 * @file    hal_arduino.h
 * @brief   zmod4xxx hardware abstraction layer for Arduino platform
 * @version 2.4.2
 * @author Renesas Electronics Corporation
 */

#ifndef _HAL_ARDUINO_H
#define _HAL_ARDUINO_H

#include "i2c_wrapper_arduino.h"
#include "zmod4xxx_types.h"

/**
 * @brief   Initialize the target hardware
 * @param   [in] dev pointer to the device
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
zmod4xxx_err init_hardware(zmod4xxx_dev_t *dev);

/**
 * @brief   deinitialize hardware if it is required
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
zmod4xxx_err deinit_hardware();

#endif //_HAL_ARDUINO_H
