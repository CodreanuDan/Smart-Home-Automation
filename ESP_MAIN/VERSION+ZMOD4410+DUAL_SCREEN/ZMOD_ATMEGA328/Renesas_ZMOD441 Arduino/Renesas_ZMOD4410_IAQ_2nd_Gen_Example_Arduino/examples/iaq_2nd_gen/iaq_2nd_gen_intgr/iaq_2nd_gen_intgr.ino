
/*##############################################################################################################################*/
/*|______________________________________INTERFATARE_ARDUINO_ESP32_SENZOR_ZMOD_4410_UART________________________________________*/
/*##############################################################################################################################*/
/*|___________________________________________________AUTOR: Dan Codereanu______________________________________________________*/
/*##############################################################################################################################*/

/*##############################################################################################################################*/
/*|________________________________________________________LIBRARIES____________________________________________________________*/
/*##############################################################################################################################*/
#include <SoftwareSerial.h>       /* Library for SoftwareSerial communication */
#include "zmod4410_config_iaq2.h" /* Configuration for IAQ 2nd Gen */
#include <zmod4xxx.h>             /* Library for ZMOD4410 sensor */
#include <zmod4xxx_hal.h>         /* Library for ZMOD4410 sensor */
#include <iaq_2nd_gen.h>          /* Library for IAQ 2nd Gen */

/*##############################################################################################################################*/
/*|________________________________________________________VARIABLES____________________________________________________________*/
/*##############################################################################################################################*/

/***************************************************_UART_COM_**************************************************************/
#define RX_PIN 10 /* RX pin */
#define TX_PIN 11 /* TX pin */

SoftwareSerial mySerial(RX_PIN, TX_PIN); /* Creation of SoftwareSerial object */

/***************************************************_STAT_DATA_*************************************************************/
String sensor_status_data; /* String for data */

/***************************************************_ZMOD_DATA_*************************************************************/

#define ARDUINO /** < selection of Hardware Abstraction Layer */

/* Global Variables */
zmod4xxx_dev_t dev; /* Device for ZMOD4410 */

/* Sensor target variables */
uint8_t zmod4xxx_status;                   /* Status of the sensor */
uint8_t prod_data[ZMOD4410_PROD_DATA_LEN]; /* Product data */
uint8_t adc_result[32] = {0};              /* ADC result */

/* IAQ 2ND GEN Variables */
iaq_2nd_gen_handle_t algo_handle;   /* Handle for IAQ 2nd Gen */
iaq_2nd_gen_results_t algo_results; /* Results of IAQ 2nd Gen */

/* Variables for results */
float valF_result_eCO2; /* Variable for eCO2 result */
float valF_result_EtOH; /* Variable for EtOH result */
float valF_result_TVOC; /* Variable for TVOC result */
float valF_result_IAQ;  /* Variable for IAQ result */

/*##############################################################################################################################*/
/*|________________________________________________________SETUP________________________________________________________________*/
/*##############################################################################################################################*/
void setup() /* Setup function */
{
    /***************************************************************************************************************************/
    /* lib_ret -> Return of Library
    * api_ret -> Return of API
    */
    int8_t lib_ret;       /* Variable for return of library */
    zmod4xxx_err api_ret; /* Variable for return of API */

    Serial.begin(9600); /* Start serial communication */
    mySerial.begin(9600); /* Start SoftwareSerial communication */
    Wire.begin();         /* Start I2C communication */

    /***************************************************************************************************************************/
    /*|______________________________________________________SETUP_ZMOD_4410__________________________________________________|*/
    /***************************************************************************************************************************/

    Serial.println(F("Starting the Sensor!"));
    api_ret = init_hardware(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during init hardware, exiting program!\n"));
        sensor_status_data = "Error";
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
        sensor_status_data = "Error";
        error_handle();
    }

    /* Preperation of sensor */
    api_ret = zmod4xxx_prepare_sensor(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during preparation of the sensor, exiting program!\n"));
        sensor_status_data = "Error";
        error_handle();
    }

    /* One time initialization of the algorithm */
    lib_ret = init_iaq_2nd_gen(&algo_handle);
    if (lib_ret) {
        Serial.println(F("Error "));
        Serial.print(lib_ret);
        Serial.println(F(" during initializing algorithm, exiting program!"));
        sensor_status_data = "Error";
        error_handle();
    }

    api_ret = zmod4xxx_start_measurement(&dev);
    if (api_ret) {
        Serial.print(F("Error "));
        Serial.print(api_ret);
        Serial.println(F(" during starting measurement, exiting program!\n"));
        sensor_status_data = "Error";
        error_handle();
    }
}



/*##############################################################################################################################*/
/*|________________________________________________________LOOP_________________________________________________________________*/
/*##############################################################################################################################*/
void loop() /* Loop function */
{
    /***************************************************************************************************************************/
    /*|_________________________________________________ZMOD_4410_MEASUREMENT_________________________________________________|*/
    /***************************************************************************************************************************/
    zmod_4410_measurement(); /* ZMOD4410 measurement */
}


/*##############################################################################################################################*/
/*|____________________________________________________ERROR_HANDLE_____________________________________________________________*/
/*##############################################################################################################################*/
void error_handle() /* Function for handling errors */
{
    // while (1) /* Infinite loop */
    // {
    //     Serial.println(F("Error occured!")); /* Print error occured */
    //     delay(1000);                         /* Delay 1 second */
    // }
    Serial.println(F("Error occured!")); /* Print error occured */
}

/*##############################################################################################################################*/
/*|______________________________________________________UART_TX________________________________________________________________*/
/*##############################################################################################################################*/
void send_trough_UART() /* Function for sending ZMOD data trough UART with software serial */
{
    /**************************************************************************************************/
    /*|__________________________________ZMOD_4410_ARDUINO_SERIAL____________________________________|*/
    Serial.print("TX_");
    Serial.print(valF_result_eCO2);
    Serial.print("_");
    Serial.print(valF_result_EtOH);
    Serial.print("_");
    Serial.print(valF_result_TVOC);
    Serial.print("_");
    Serial.print(valF_result_IAQ);
    Serial.print("_");
    Serial.print(sensor_status_data);
    Serial.println();

    /**************************************************************************************************/
    /*|__________________________________ZMOD_4410_ARDUINO_UART______________________________________|*/
    /**************************************************************************************************/
    mySerial.print("TX_");
    mySerial.print(valF_result_eCO2);
    mySerial.print("_");
    mySerial.print(valF_result_EtOH);
    mySerial.print("_");
    mySerial.print(valF_result_TVOC);
    mySerial.print("_");
    mySerial.print(valF_result_IAQ);
    mySerial.print("_");
    mySerial.print(sensor_status_data);
    mySerial.println();
}

/*##############################################################################################################################*/
/*|_________________________________________________ZMOD_4410_MEASUREMENT_______________________________________________________*/
/*##############################################################################################################################*/
void zmod_4410_measurement() /* Function for ZMOD4410 measurement */
{

    /* lib_ret -> Return of Library
     * api_ret -> Return of API
     */
    int8_t lib_ret;               /* Variable for return of library */
    zmod4xxx_err api_ret;         /* Variable for return of API */
    uint32_t polling_counter = 0; /* Counter for polling */

    // unsigned long currentTime = millis();
    // unsigned long elapsedTime = currentTime - previousTime;

    do
    {
        api_ret = zmod4xxx_read_status(&dev, &zmod4xxx_status); /* Read status */
        if (api_ret)                                            /* If there is an error */
        {
            Serial.print(F("Error "));
            Serial.print(api_ret); /* Print error */
            Serial.println(F(" during read of sensor status, exiting program!\n"));
            sensor_status_data = "Error";
            error_handle(); /* Handle error */
        }
        polling_counter++;                                                                                           /* Increment polling counter */
        dev.delay_ms(200);                                                                                           /* Delay 200 ms */
    } while ((zmod4xxx_status & STATUS_SEQUENCER_RUNNING_MASK) && (polling_counter <= ZMOD4410_IAQ2_COUNTER_LIMIT)); /* While sequencer is running and polling counter is less than limit */

    if (ZMOD4410_IAQ2_COUNTER_LIMIT <= polling_counter) /* If polling counter is greater than limit */
    {
        api_ret = zmod4xxx_check_error_event(&dev); /* Check error event */
        if (api_ret)                                /* If there is an error */
        {
            Serial.print(F("Error "));
            Serial.print(api_ret); /* Print error */
            Serial.println(F(" during read of sensor status, exiting program!\n"));
            sensor_status_data = "Error";
            error_handle(); /* Handle error */
        }
        Serial.print(F("Error"));
        Serial.print(ERROR_GAS_TIMEOUT); /* Print error */
        Serial.print(F(" exiting program!"));
        sensor_status_data = "Error";
        error_handle(); /* Handle error */
    }
    else
    {
        polling_counter = 0; /* Set polling counter to 0 */
    }

    api_ret = zmod4xxx_read_adc_result(&dev, adc_result); /* Read ADC result */
    if (api_ret)                                          /* If there is an error */
    {
        Serial.print(F("Error "));
        Serial.print(api_ret); /* Print error */
        Serial.println(F(" during read of ADC results, exiting program!\n"));
        sensor_status_data = "Error";
        error_handle(); /* Handle error */
    }

    /* calculate the algorithm */
    lib_ret = calc_iaq_2nd_gen(&algo_handle, &dev, adc_result, &algo_results); /* Calculate IAQ 2nd Gen */
    if ((lib_ret != IAQ_2ND_GEN_OK) && (lib_ret != IAQ_2ND_GEN_STABILIZATION)) /* If there is an error */
    {
        Serial.println(F("Error when calculating algorithm, exiting program!"));
        sensor_status_data = "Error";
    }
    else
    {
      Serial.println(F("*********** Measurements ***********"));
      Serial.println(algo_results.rmox[0]);

      valF_result_IAQ  = algo_results.iaq;   /* Add IAQ result to variable */
      valF_result_TVOC = algo_results.tvoc;  /* Add TVOC result to variable */
      valF_result_eCO2 = algo_results.eco2;  /* Add eCO2 result to variable */
      valF_result_EtOH = algo_results.etoh;  /* Add EtOH result to variable */
      sensor_status_data = "SensorOK";

      /***************************************************************************************************************************/
      /*|__________________________________________________SEND_TRHOUGH_UART____________________________________________________|*/
      /***************************************************************************************************************************/
      send_trough_UART(); /* Send data trough UART */

    }

    if (lib_ret == IAQ_2ND_GEN_STABILIZATION) /* If IAQ 2nd Gen is stabilized */
    {
        Serial.println(F("Warmup!")); /* Print warmup */
    }
    else
    {
        Serial.println(F("Valid!")); /* Print valid */
    }

    /* wait 1.99 seconds before starting the next measurement */
    dev.delay_ms(1990); /* Delay 1990 ms */
    // arrayIndex += 1;

    /* start a new measurement before result calculation */
    api_ret = zmod4xxx_start_measurement(&dev); /* Start measurement */
    if (api_ret)                                /* If there is an error */
    {
        Serial.println(F("Error when starting measurement, exiting program!\n")); /* Print error */
    }
}


/*##############################################################################################################################*/