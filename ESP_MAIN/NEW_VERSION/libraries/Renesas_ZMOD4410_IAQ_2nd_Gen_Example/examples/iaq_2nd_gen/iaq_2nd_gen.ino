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
 * @file    iaq_2nd_gen.ino
 * @brief   This is an Arduino example for the ZMOD4410 gas sensor using the IAQ 2nd Gen library.
 * @version 2.2.0
 * @author Renesas Electronics Corporation
 */

#define ARDUINO /** < selection of Hardware Abstraction Layer */

#include <zmod4410_config_iaq2.h>
#include <zmod4xxx.h>
#include <zmod4xxx_hal.h>
#include <iaq_2nd_gen.h>

void error_handle();

/* Global Variables */
zmod4xxx_dev_t dev;

/* Sensor target variables */
uint8_t zmod4xxx_status;
uint8_t prod_data[ZMOD4410_PROD_DATA_LEN];
uint8_t adc_result[32] = { 0 };

/* IAQ 2ND GEN Variables */
iaq_2nd_gen_handle_t algo_handle;
iaq_2nd_gen_results_t algo_results;

void setup()
{
    /* lib_ret -> Return of Library
     * api_ret -> Return of API
     */
    int8_t lib_ret;
    zmod4xxx_err api_ret;

    Serial.begin(9600);

    Serial.println(F("Starting the Sensor!"));
    api_ret = init_hardware(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during init hardware, exiting program!\n"));
        error_handle();
    }

    dev.i2c_addr = ZMOD4410_I2C_ADDR;
    dev.pid = ZMOD4410_PID;
    dev.init_conf = &zmod_sensor_type[INIT];
    dev.meas_conf = &zmod_sensor_type[MEASUREMENT];
    dev.prod_data = prod_data;

    api_ret = zmod4xxx_read_sensor_info(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(
            F(" during reading sensor information, exiting program!\n"));
        error_handle();
    }

    /* Preperation of sensor */
    api_ret = zmod4xxx_prepare_sensor(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during preparation of the sensor, exiting program!\n"));
        error_handle();
    }

    /* One time initialization of the algorithm */
    lib_ret = init_iaq_2nd_gen(&algo_handle);
    if (lib_ret) {
        Serial.println(F("Error "));
        Serial.print(lib_ret);
        Serial.println(F(" during initializing algorithm, exiting program!"));
        error_handle();
    }

    api_ret = zmod4xxx_start_measurement(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during starting measurement, exiting program!\n"));
        error_handle();
    }
}

void loop()
{
    /* lib_ret -> Return of Library
     * api_ret -> Return of API
     */
    int8_t lib_ret;
    zmod4xxx_err api_ret;
    uint32_t polling_counter = 0;


    do {
        api_ret = zmod4xxx_read_status(&dev, &zmod4xxx_status);
        if (api_ret) {
            Serial.print(F("Error "));
            Serial.print(api_ret);
            Serial.println(F(" during read of sensor status, exiting program!\n"));
            error_handle();
        }
        polling_counter++;
        dev.delay_ms(200);
    } while ((zmod4xxx_status & STATUS_SEQUENCER_RUNNING_MASK)&&
             (polling_counter <= ZMOD4410_IAQ2_COUNTER_LIMIT));

    if (ZMOD4410_IAQ2_COUNTER_LIMIT <= polling_counter) {
        api_ret = zmod4xxx_check_error_event(&dev);
        if (api_ret) {
            Serial.print(F("Error "));
            Serial.print(api_ret);
            Serial.println(F(" during read of sensor status, exiting program!\n"));
            error_handle();
        }
        Serial.print(F("Error"));
        Serial.print(ERROR_GAS_TIMEOUT);
        Serial.print(F(" exiting program!"));
        error_handle();
    } else {
        polling_counter = 0;
    }

    api_ret = zmod4xxx_read_adc_result(&dev, adc_result);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during read of ADC results, exiting program!\n"));
        error_handle();
    }

    /* calculate the algorithm */
    lib_ret = calc_iaq_2nd_gen(&algo_handle, &dev, adc_result, &algo_results);
    if ((lib_ret != IAQ_2ND_GEN_OK) && (lib_ret != IAQ_2ND_GEN_STABILIZATION)) {
        Serial.println(F("Error when calculating algorithm, exiting program!"));
    } else {
        Serial.println(F("*********** Measurements ***********"));
        for (int i = 0; i < 13; i++) {
            Serial.print(F(" Rmox["));
            Serial.print(i);
            Serial.print(F("] = "));
            Serial.print(algo_results.rmox[i] / 1e3);
            Serial.println(F(" kOhm"));
        }

        Serial.print(F(" log_Rcda = "));
        Serial.print(algo_results.log_rcda);
        Serial.println(F(" logOhm"));
        Serial.print(F(" EtOH = "));
        Serial.print(algo_results.etoh);
        Serial.println(F(" ppm"));
        Serial.print(F(" TVOC = "));
        Serial.print(algo_results.tvoc);
        Serial.println(F(" mg/m^3"));
        Serial.print(F(" eCO2 = "));
        Serial.print(algo_results.eco2);
        Serial.println(F(" ppm"));
        Serial.print(F(" IAQ  = "));
        Serial.println(algo_results.iaq);

        if (lib_ret == IAQ_2ND_GEN_STABILIZATION) {
            Serial.println(F("Warmup!"));
        } else {
            Serial.println(F("Valid!"));
        }
    }

    /* wait 1.99 seconds before starting the next measurement */
    dev.delay_ms(1990);

    /* start a new measurement before result calculation */
    api_ret = zmod4xxx_start_measurement(&dev);
    if (api_ret) {
        Serial.println(F("Error when starting measurement, exiting program!\n"));
    }
}

void error_handle()
{
    while (1);
}
