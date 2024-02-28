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
//

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

#include "MQTT_COM_TASK_FUNCTIONS.h"                                                                                  /*  MQTT communication standard: reconnect, callback, publish */
#include "SETUP_WIFI_TASK_FUNCTIONS.h"                                                                                /*  Connecting to WiFI networks */
#include "SCAN_I2C_TASK_FUNCTIONS.h"                                                                                  /*  Scanning for devices on I2C bus */

#include "HEAT_CONTROL_TASK_FUNCTIONS.h"                                                                              /*  Controls the heating via relay */
#include "FAN_CONTROL_TASK_FUNCTIONS.h"                                                                               /*  Controls the cooling via relay */

#include "SHT_TASK_FUNCTIONS.h"                                                                                       /*  Reads data from SHT21 sensor */
#include "MQ2_TASK_FUNCTIONS.h"                                                                                       /*  Reads data from MQ2 sensor   */

#include "SHT_DISPLAY_TASK_FUNCTIONS.h"                                                                               /*  Displays data from the SHT21 sensor */  
#include "DISPLAY_STATE_TASK_FUNCTIONS.h"                                                                             /*  Displays the state of the climate control system*/

#include "DEFINES.h"                                                                                                  /*  Includes all the variables used on the program*/


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
  Serial.begin(115200);                                                                                               /* Setting the Baud Rate to 115200 and Serial.begin() */
  Serial.println("=============================================================================================");
  Serial.println("  ");
  Serial.println("<0> SETUP BEGIN <1>");
  Serial.println("  ");
  Serial.println("<0> SERIAL BEGIN <1>");
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*_____________________________________________________________________WIRE_BEGIN_(I2C):___________________________________________________________________*/   
  /***********************************************************************************************************************************************************/
  Wire.begin();                                                                                                       /* Begin communication with I2C bus */
  Serial.println("<0> WIRE BEGIN <1>");
  delay(tc_moduleSetup_delay);

  //________________________________________________INFO__
  Serial.print("<INFO> setup() running on core ");
  Serial.println(xPortGetCoreID());                                                                                   /* See on wich core setup() runs */
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*_________________________________________________________________________PINMODE_INIT:___________________________________________________________________*/   
  /***********************************************************************************************************************************************************/
  Serial.println("<*> PINMODE INIT <*>");

  pinMode(pin_heatRelayPin, OUTPUT);                                                                                  /* >> pinMode: heatRelayPin: as OUTPUT. (Controls the toggle of the HEAT ON/OFF)*/
  Serial.print("  >> pinMode: heatRelayPin:");
  Serial.print(pin_heatRelayPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_fanRelayPin, OUTPUT);                                                                                   /* >> pinMode: fanRelayPin: as OUTPUT. (Controls the toggle of the FAN ON/OFF) */
  Serial.print("  >> pinMode: fanRelayPin:");
  Serial.print(pin_fanRelayPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_ledBuiltInPin, OUTPUT);                                                                                 /* >> pinMode: ledBuiltInPin: as OUTPUT. (Signals if connection to MQTT Server is succsesful*/
  digitalWrite(pin_ledBuiltInPin, LOW);
  Serial.print("  >> pinMode: ledBuiltInPin:");
  Serial.print(pin_ledBuiltInPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_ledRedPin, OUTPUT);                                                                                     /* >> pinMode: ledRedPin: as OUTPUT. (In role of light control)*/
  digitalWrite(pin_ledRedPin, LOW);
  Serial.print("  >> pinMode: ledRedPin:");
  Serial.print(pin_ledRedPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  //__PINMODE_INIT_END__
  Serial.println("<**> PINMODE INIT FINISHED<**>");
  delay(tc_moduleSetup_delay);

  /***********************************************************************************************************************************************************/
  /*____________________________________________________________________________MQTT_INIT:___________________________________________________________________*/   
  /***********************************************************************************************************************************************************/

  //__MQTT_SERVER_CONNECTION__
  Serial.println("<*> MQTT: --> Establish connection to MQTT server, PORT 1883 <*>");
  client.setServer(mqtt_server,1883);                                                                                 /*  Set the MQTT Server IP and the PORT */
  delay(tc_objCreate_delay);

  //__MQTT_CALLBACK_SET__
  Serial.println("<*> MQTT: --> Set callback function<*>");
  client.setCallback(callback);                                                                                       /*  Set callback function: Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */
  delay(tc_objCreate_delay);


/*###################################################################################################################################################################*/
/*___________________________________________________________________________CREATING_TASKS:_________________________________________________________________________*/
/*###################################################################################################################################################################*/
  /* --> Core ID identification : [#] - Core 0 | [##] - Core 1 (Serial monitor) */
  /* --> Core 0: Control Tasks will run on Core 0 */
  /* --> Core 1: Communication Tasks & Diag Tasks will run on Core 1 */

  Serial.println("<*> CREATING THE TASKS <*>");

  /***********************************************************************************************************************************************************/
  /*___________________________________________________________________CORE_0_TASKS_(Control_Tasks):_________________________________________________________*/   
  /***********************************************************************************************************************************************************/

  //__________________________SENSOR_TASKS:

  /******************************************************************************************/
  /* Reads data from the SHT21sensor. It's data is used in the other tasks                  */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_shtReadTask,                /* Task function. */
    "shtReadTask",                /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_shtReadTask,                /* Priority of the task. */
    &h_shtReadTask,               /* Task handle to keep track of created task */
    0                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: shtReadTask");               
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Reads data from the MQ2 sensor. It's data is used in the other tasks                   */
  /******************************************************************************************/
   xTaskCreatePinnedToCore(
    T_mqReadTask,                 /* Task function. */
    "mqReadTask",                 /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_mqReadTask,                 /* Priority of the task. */
    &h_mqReadTask,                /* Task handle to keep track of created task */
    0                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: mqReadTask");               
  delay(tc_objCreate_delay);

  //_________________________DISPLAY_TASKS:

  /******************************************************************************************/
  /* Displays data from the sensor. Uses data from shtReadTask()                            */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_shtDisplayTask,             /* Task function. */
    "shtDisplayTask",             /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_shtDisplayTask,             /* Priority of the task. */
    &h_shtDisplayTask,            /* Task handle to keep track of created task */
    0                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: shtDisplayTask");            
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Displays the current state of the system (num & char)                                  */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_displayStateTask,           /* Task function. */
    "displayStateTask",           /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_displayStateTask,           /* Priority of the task. */
    &h_displayStateTask,          /* Task handle to keep track of created task */
    0                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: displayStateTask");          
  delay(tc_objCreate_delay);

  //_________________________CONTROL_TASKS:

  /******************************************************************************************/
  /* Controls the Fan. Calls shtReadTask() & toggleFan(bool state).                         */
  /* Can change the currentState between OFF <--> COOLING                                   */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_fanControlTask,             /* Task function. */
    "fanControlTask",             /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_fanControlTask,             /* Priority of the task. */
    &h_fanControlTask,            /* Task handle to keep track of created task */
    0                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: fanControlTask");            
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Controls the Heating. Calls shtReadTask() & toggleHeat(bool state).                    */
  /* Can change the currentState between OFF <--> HEATING                                   */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_heatControlTask,            /* Task function. */
    "heatControlTask",            /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_heatControlTask,            /* Priority of the task. */
    &h_heatControlTask,           /* Task handle to keep track of created task */
    0                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: heatControlTask");           
  delay(tc_objCreate_delay);
  

  /***********************************************************************************************************************************************************/
  /*_________________________________________________CORE_1_TASKS_(Communication_Tasks_&_Diag_Tasks):________________________________________________________*/
  /***********************************************************************************************************************************************************/    

  //_________________________DIAG_TASKS:

  /******************************************************************************************/
  /* Scans connection to I2C Adresses, returns them                                         */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_scanI2CTask,                /* Task function. */
    "scanI2CTask",                /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_scanI2CTask,                /* Priority of the task. */
    &h_scanI2CTask,               /* Task handle to keep track of created task */
    1                             /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: scanI2CTask");               
  delay(tc_objCreate_delay);

  //__________________________COM_TASKS:

  /******************************************************************************************/
  /* Task responsable for connecting to WiFi Networks                                       */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_setupWifiTask,              /* Task function. */
    "setupWifiTask",              /* String with name of task. */
    5000,                         /* Stack size in bytes. */
    NULL,                         /* Parameter passed as input of the task */
    p_setupWiFiTask,              /* Priority of the task. */
    &h_setupWiFiTask,             /* Task handle to keep track of created task */
    1                             /* pin task to core 1 */
  );
  Serial.println("  >> Created Task: setupWifiTask");            
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Task responsable for MQTT communication: reconnecting to server,                       */
  /* publishing data, making decisions based on received data and sending updates to system */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_MQTT_CommunciationTask,      /* Task function. */
    "MQTT_CommunciationTask",      /* String with name of task. */
    5000,                          /* Stack size in bytes. */
    NULL,                          /* Parameter passed as input of the task */
    p_MQTT_CommunciationTask,      /* Priority of the task. */
    &h_MQTT_CommunciationTask,     /* Task handle to keep track of created task */
    1                              /* pin task to core 1 */
  );
  Serial.println("  >> Created Task: MQTT_CommunciationTask");    
  delay(tc_objCreate_delay);

  Serial.println("<**> TASKS CREATED <**>");
  delay(tc_moduleSetup_delay);


  /*********************************************************************************************************************************************************************/  
  /*__________________________________________________________________________SETUP_FINISHED___________________________________________________________________________*/
  /*********************************************************************************************************************************************************************/  
  Serial.println("<!> SETUP FINISHED <!>");
  Serial.println("=============================================================================================");
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


