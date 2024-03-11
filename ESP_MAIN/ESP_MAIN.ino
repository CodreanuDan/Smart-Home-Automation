/*###################################################################################################################################################################*/
/*_____________________________________________________________________________DESCRIPTION:__________________________________________________________________________*/
/*###################################################################################################################################################################*/
//  v1.0 --> task in loop (dupa numeTask si taskPrio) - mai mult functii clasice, un fel de pseudo-task :)))
//  v2.0 --> task in loop (dupa taskPrio) - la fel ca mai sus
//  v3.0 --> Proper Task declarat in setup() - task-uri din freeRtos --> implementare RTOS in soft !
//  v4.0 --> TaskPinned to Core - delimitare intre task-urile de control si cele de comunicatie
//  v5.0 --> On Real ESP; dht -> sht; - implementare pe placuta fizica + identificare adrese I2C (SHT 21)
//  v6.0 --> Introducere WiFi connection
//  v7.0 --> Introducere comunicatie via MQTT
//  v8.0 --> De adaugat: vizualizare in dashboard a Starii sistemului X + Current Time via NTP + Thershold via manual control X : TODO !!! --> mai am NTP !!!
//  v9.0 --> Creat module spearate pentru fiecare actiune si un fiser de define


/*###################################################################################################################################################################*/
/*______________________________________________________________________________INCLUDES:____________________________________________________________________________*/
/*###################################################################################################################################################################*/

/*************************************************************************
/*___________________________LIBRARIES:_________________________________*/ 

#include <SHT21.h>                                                                                                    /* SHT 21 Sensor library */
#include <Wire.h>                                                                                                     /* I2C bus library */

#include <PubSubClient.h>                                                                                             /* MQTT Communication Protocol library */
#include <WiFi.h>                                                                                                     /* WiFi library */

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "nvs_flash.h"

/*************************************************************************
/*_____________________________MODULES:_________________________________*/  

#include "MQTT_COM_TASK_FUNCTIONS.h"                                                                                  /*  MQTT communication standard: reconnect, callback */
#include "MQTT_PUB_TASK_FUNCTIONS.h"                                                                                  /*  MQTT communication standard: publish */
#include "SETUP_WIFI_TASK_FUNCTIONS.h"                                                                                /*  Connecting to WiFI networks */
#include "SCAN_I2C_TASK_FUNCTIONS.h"                                                                                  /*  Scanning for devices on I2C bus */

#include "HEAT_CONTROL_TASK_FUNCTIONS.h"                                                                              /*  Controls the heating via relay */
#include "FAN_CONTROL_TASK_FUNCTIONS.h"                                                                               /*  Controls the cooling via relay */

#include "SHT_TASK_FUNCTIONS.h"                                                                                       /*  Reads data from SHT21 sensor */
// #include "MQ2_TASK_FUNCTIONS.h"                                                                                       /*  Reads data from MQ2 sensor   */

#include "SENSORS_DISPLAY_TASK_FUNCTIONS.h"                                                                           /*  Displays data from the sensors                  */  
#include "DISPLAY_STATE_TASK_FUNCTIONS.h"                                                                             /*  Displays the state of the climate control system*/

#include "DEFINES.h"                                                                                                  /*  Includes all the variables used on the program*/
#include "ESP_TASKS.h"                                                                                                /*  Creates the tasks */ 
#include "MODULE_INIT.h"                                                                                              /*  Module init */


/*###################################################################################################################################################################*/
/*____________________________________________________________________________DECALARATIONS:_________________________________________________________________________*/
/*###################################################################################################################################################################*/


/*###################################################################################################################################################################*/
/*________________________________________________________________________________SETUP:_____________________________________________________________________________*/
/*###################################################################################################################################################################*/

void setup() 
{
  
  /***********************************************************************************************************************************************************/
  /*_________________________________________________________________________SERIAL_BEGIN:___________________________________________________________________*/   
  /***********************************************************************************************************************************************************/
  Serial.begin(115200);                                                                                             /* Setting the Baud Rate to 115200 and Serial.begin() */

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
  wireInit();                                                                                                       /* Start communication on I2C bus*/
  delay(tc_moduleSetup_delay);
  
  /***********************************************************************************************************************************************************/
  /*______________________________________________________________________PINMODE_INIT:______________________________________________________________________*/   
  /***********************************************************************************************************************************************************/
  pinInit();                                                                                                        /* Set pin modes and pins*/
  delay(tc_moduleSetup_delay);
 
  /***********************************************************************************************************************************************************/
  /*________________________________________________________________________MQ2_CAL:_________________________________________________________________________*/   
  /***********************************************************************************************************************************************************/
  mq2Init();                                                                                                        /* Inits the mq2 sensor*/
  delay(tc_moduleSetup_delay);
  
  /***********************************************************************************************************************************************************/
  /*_______________________________________________________________________MQTT_INIT:________________________________________________________________________*/   
  /***********************************************************************************************************************************************************/
  mqttInit();                                                                                                       /* Inits mqtt broker and sets the callback function*/
  delay(tc_moduleSetup_delay);
  
/*###################################################################################################################################################################*/
/*______________________________________________________________________CREATING_TASKS:______________________________________________________________________________*/
/*###################################################################################################################################################################*/
  /* --> Core ID identification : [#] - Core 0 | [##] - Core 1 (Serial monitor) */
  /* --> Core 0: Control Tasks will run on Core 0 */
  /* --> Core 1: Communication Tasks & Diag Tasks will run on Core 1 */
  espTaskCreate();                                                                                                  /* Creates the esp_tasks*/
  delay(tc_moduleSetup_delay);

  /*********************************************************************************************************************************************************************/  
  /*__________________________________________________________________________SETUP_FINISHED___________________________________________________________________________*/
  /*********************************************************************************************************************************************************************/
  Serial.println("###########################################################################################################################");  
  Serial.println("#|_____________________________________________________SETUP_FINISHED____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");
  delay(tc_moduleSetup_delay);
 
}


/*###################################################################################################################################################################*/
/*_______________________________________________________________________________LOOP:_______________________________________________________________________________*/
/*###################################################################################################################################################################*/

void loop()
{
  /* EMPTY LOOP FUNCTION, DUE TO USAGE OF FREE_RTOS TASKS AND FEATURES */
}


