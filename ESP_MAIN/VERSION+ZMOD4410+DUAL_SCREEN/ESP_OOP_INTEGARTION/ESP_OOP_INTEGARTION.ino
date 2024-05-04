/*###################################################################################################################################################################*/
/*_____________________________________________________________________________DESCRIPTION:__________________________________________________________________________*/
/*###################################################################################################################################################################*/
/**
 * @brief UPDATE LIST:

  v1.0 --> task in loop (dupa numeTask si taskPrio) - mai mult functii clasice, un fel de pseudo-task :)))
  v2.0 --> task in loop (dupa taskPrio) - la fel ca mai sus
  v3.0 --> Proper Task declarat in setup() - task-uri din freeRtos --> implementare RTOS in soft !
  v4.0 --> TaskPinned to Core - delimitare intre task-urile de control si cele de comunicatie
  v5.0 --> On Real ESP; dht -> sht; - implementare pe placuta fizica + identificare adrese I2C (SHT 21)
  v6.0 --> Introducere WiFi connection
  v7.0 --> Introducere comunicatie via MQTT
  v8.0 --> De adaugat: vizualizare in dashboard a Starii sistemului X + Current Time via NTP + Thershold via manual control X : TODO !!!(facut) --> mai am NTP !!!(nu mai e nevoie!)
  v9.0 --> Creat module spearate pentru fiecare actiune si un fiser de define
  v10.0 --> Integrat senzor BME 680
  v11.0 --> Creat clase de obiecte pentru Module
  v12.0 --> Integrare 2 magistrale pentru I2C : una pentru senzori alta pentru Arduino si alta pentru senzori --> de facut --> nu prea merge
  v13.0 --> Adugat temp exterioara
  v14.0 --> Adaugat controlul temperaturii cu tolernata
  v15.0 --> Adaugat UART; Adauga Ecrane LCD si OLED; 


*/

/**
 * @file ESP_OOP_INTEGRATION.ino
 * @brief Main file for the smart home control & monitoring system.
 */

/*###################################################################################################################################################################*/
/*______________________________________________________________________________INCLUDES:____________________________________________________________________________*/
/*###################################################################################################################################################################*/

/************************************************************************/
/*___________________________LIBRARIES:_________________________________*/
/************************************************************************/

/**
 * @brief Include the SHT21 Sensor Library
 *
 * This library provides functions to interact with the SHT21 humidity and temperature sensor.
 */
#include <SHT21.h>

/**
 * @brief Include the Adafruit Unified Sensor Driver
 *
 * This library provides a common interface for sensors to make them easier to use.
 * It's used by the Adafruit_BME680 library to provide the sensor readings.
 */
#include <Adafruit_Sensor.h>

/**
 * @brief Include the Adafruit BME680 Library
 *
 * This library provides functions to interact with the BME680 environmental sensor.
 * The BME680 sensor provides temperature, humidity, pressure and gas readings.
 */
#include "Adafruit_BME680.h"

/**
 * @brief Include the Wire Library
 *
 * This library provides functions to communicate with I2C devices.
 * It's used to communicate with the SHT21 and BME680 sensors.
 */
#include <Wire.h>

/**
 * @brief Include the PubSubClient Library
 *
 * This library provides functions to communicate with MQTT servers.
 * It's used to publish the sensor readings to an MQTT server.
 */
#include <PubSubClient.h>

/**
 * @brief Include the WiFi Library
 *
 * This library provides functions to connect to a WiFi network.
 * It's used to connect the ESP32 to the internet so it can communicate with the MQTT server.
 */
#include <WiFi.h>

/**
 * @brief Include the Adafruit SSD1306 Library
 *
 * This library provides functions to interact with the SSD1306 OLED display.
 * It's used to display the sensor readings on the OLED display.
 */
#include <Adafruit_SSD1306.h>

/**
 * @brief Include the Adafruit GFX Library
 *
 * This library provides functions to draw shapes and text on displays.
 * It's used by the Adafruit SSD1306 library to draw text on the OLED display.
 */
#include <Adafruit_GFX.h>

/**
 * @brief Include the LiquidCrystal_I2C Library
 *
 * This library provides functions to interact with the I2C LCD display.
 * It's used to display the sensor readings on the LCD display.
 */
#include <LiquidCrystal_I2C.h> /* Include the LiquidCrystal_I2C library */

/************************************************************************/
/*_____________________________MODULES:_________________________________*/
/************************************************************************/

/**
 * @brief Modules used in communication
 */

#include "MQTT_COM_TASK_FUNCTIONS.h"   /*  MQTT communication standard: reconnect, callback */
#include "MQTT_PUB_TASK_FUNCTIONS.h"   /*  MQTT communication standard: publish */
#include "SETUP_WIFI_TASK_FUNCTIONS.h" /*  Connecting to WiFI networks */
#include "SCAN_I2C_TASK_FUNCTIONS.h"   /*  Scanning for devices on I2C bus */
#include "UART_COM_TASK_FUNCTIONS.h"   /*  Communication trough UART 2 bus*/
#include "I2C_LCD_TASK_FUNCTIONS.h"    /*  Communication trough I2C bus with LCD and OLED*/

/**
 * @brief Modules used in climate control.
 */
#include "DISPLAY_STATE_TASK_FUNCTIONS.h" /*  Displays the state of the climate control system*/
#include "HEAT_CONTROL_TASK_FUNCTIONS.h"  /*  Controls the heating via relay */
#include "FAN_CONTROL_TASK_FUNCTIONS.h"   /*  Controls the cooling via relay */

/**
 * @brief Modules used for sensor readings.
 */
#include "SENSORS_DISPLAY_TASK_FUNCTIONS.h" /*  Displays data from the sensors        */
#include "SHT21_BME680_Readings.h"          /*  Reads data from SHT21, BME680 sensors */

/**
 * @brief Modules used in setup of the system: creating task, initialisation of I2C buses, void setup() operations etc.
 */
#include "DEFINES.h"     /*  Includes all the variables and macros used on the program */
#include "ESP_TASKS.h"   /*  Creates the tasks */
#include "MODULE_INIT.h" /*  Module init */

/*###################################################################################################################################################################*/
/*____________________________________________________________________________DECALARATIONS:_________________________________________________________________________*/
/*###################################################################################################################################################################*/

bool f_setupInitFlag;
/*###################################################################################################################################################################*/
/*________________________________________________________________________________SETUP:_____________________________________________________________________________*/
/*###################################################################################################################################################################*/
void setup()
{
  f_setupInitFlag = false;
  /***********************************************************************************************************************************************************/
  /*_________________________________________________________________________SERIAL_BEGIN:___________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  Serial.begin(115200); /* Setting the Baud Rate to 115200 and Serial.begin() */

  Serial.print("<INFO> setup() running on core ");
  Serial.println(xPortGetCoreID()); /* See on wich core setup() runs */
  delay(tc_moduleSetup_delay);

  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________SERIAL_BEGIN____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");
  delay(tc_moduleSetup_delay);

  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________SETUP_BEGIN_____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*_____________________________________________________________________WIRE_BEGIN_(I2C):___________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation of I2C buses.
   */
  setupInit.wireInit(); /* Start communication on I2C bus*/
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*_______________________________________________________________________UART_2_INIT:______________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation of UART 2 bus.
   */
  setupInit.uart2Init(); /* Start communication on UART 2 bus*/
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*______________________________________________________________________PINMODE_INIT:______________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation of the pins.
   */
  setupInit.pinInit(); /* Set pin modes and pins*/
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*_______________________________________________________________________MQTT_INIT:________________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation MQTT client and callback function.
   */
  setupInit.mqttInit(); /* Inits mqtt broker and sets the callback function*/
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*______________________________________________________________________MUTEX_INIT:________________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation of the mutexes.
   */
  setupInit.mutexInit(); /* Inits the mutexes */
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*______________________________________________________________________LCD_INIT:__________________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation of the LCD and OLED displays.
   */
  setupInit.lcdInit(); /* Inits the LCD and OLED displays */
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*______________________________________________________________________OLED_INIT:_________________________________________________________________________*/
  /***********************************************************************************************************************************************************/
  /**
   * @brief Initialisation of the OLED display.
   */
  setupInit.oledInit(); /* Inits the OLED display */
  delay(tc_moduleSetup_delay);

  /*###################################################################################################################################################################*/
  /*______________________________________________________________________CREATING_TASKS:______________________________________________________________________________*/
  /*###################################################################################################################################################################*/
  /* --> Core ID identification : [#] - Core 0 | [##] - Core 1 (Serial monitor) */
  /* --> Core 0: Control Tasks will run on Core 0 */
  /* --> Core 1: Communication Tasks & Diag Tasks will run on Core 1 */
  /**
   * @brief Creates Free RTOS Tasks.
   */
  taskInit.espTaskCreate(); /* Creates the esp_tasks*/
  delay(tc_moduleSetup_delay);

  /*********************************************************************************************************************************************************************/
  /*__________________________________________________________________________SETUP_FINISHED___________________________________________________________________________*/
  /*********************************************************************************************************************************************************************/
  Serial.println("###########################################################################################################################");
  Serial.println("#|_____________________________________________________SETUP_FINISHED____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");
  delay(tc_moduleSetup_delay);
  lcd.clear();
  f_setupInitFlag = true;
}

/*###################################################################################################################################################################*/
/*_______________________________________________________________________________LOOP:_______________________________________________________________________________*/
/*###################################################################################################################################################################*/
void loop()
{
  /* EMPTY LOOP FUNCTION, DUE TO USAGE OF FREE_RTOS TASKS AND FEATURES */
}
