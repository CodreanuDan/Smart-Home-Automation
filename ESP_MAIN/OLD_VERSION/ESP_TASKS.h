/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: ESP_TASKS.h */

/*   ---> Modulul o functie care creeaza task-urile: void espTaskCreate()*/

#ifndef ESP_TASKS
#define ESP_TASKS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/


/*__MODULES__*/
#include "DEFINES.h"                                             /*  Includes all the variables used on the program*/

/*##########################################################################################################################*/
/*|______________________________________________ESP_TASK_CREATE_FUNCTION:_________________________________________________|*/
/*##########################################################################################################################*/
void espTaskCreate()                                             /* Function that creates the tasks */
{

  /*###################################################################################################################################################################*/
  /*___________________________________________________________________________CREATING_TASKS:_________________________________________________________________________*/
  /*###################################################################################################################################################################*/
  /* --> Core ID identification : [#] - Core 0 | [##] - Core 1 (Serial monitor) */
  /* --> Core 0: Control Tasks will run on Core 0 */
  /* --> Core 1: Communication Tasks & Diag Tasks will run on Core 1 */

  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________TASK_CREATE_____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  /***********************************************************************************************************************************************************/
  /*___________________________________________________________________CORE_0_TASKS_(Control_Tasks):_________________________________________________________*/   
  /***********************************************************************************************************************************************************/

  //__________________________SENSOR_TASKS:

  /******************************************************************************************/
  /* Reads data from the SHT21sensor. It's data is used in the other tasks                  */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_shtReadTask,                                              /* Task function. */
    "shtReadTask",                                              /* String with name of task. */
    5000,                                                       /* Stack size in bytes. */
    NULL,                                                       /* Parameter passed as input of the task */
    p_shtReadTask,                                              /* Priority of the task. */
    &h_shtReadTask,                                             /* Task handle to keep track of created task */
    0                                                           /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: shtReadTask");               
  delay(tc_objCreate_delay);

  // /******************************************************************************************/
  // /* Reads data from the MQ2 sensor. It's data is used in the other tasks                   */
  // /******************************************************************************************/
  // xTaskCreatePinnedToCore(
  //   T_mqReadTask,                                               /* Task function. */
  //   "mqReadTask",                                               /* String with name of task. */
  //   5000,                                                       /* Stack size in bytes. */
  //   NULL,                                                       /* Parameter passed as input of the task */
  //   p_mqReadTask,                                               /* Priority of the task. */
  //   &h_mqReadTask,                                              /* Task handle to keep track of created task */
  //   0                                                           /* pin task to core 0 */
  // );
  // Serial.println("  >> Created Task: mqReadTask");               
  // delay(tc_objCreate_delay);

  //_________________________DISPLAY_TASKS:

  /******************************************************************************************/
  /* Displays data from the sensor. Uses data from: --> shtReadTask()                       */
  /*                                                --> mqReadTask()                        */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_sensorsDisplayTask,                                       /* Task function. */
    "sensorsDisplayTask",                                       /* String with name of task. */
    5000,                                                       /* Stack size in bytes. */
    NULL,                                                       /* Parameter passed as input of the task */
    p_sensorsDisplayTask,                                       /* Priority of the task. */
    &h_sensorsDisplayTask,                                      /* Task handle to keep track of created task */
    0                                                           /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: sensorsDisplayTask");            
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Displays the current state of the system (num & char)                                  */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_displayStateTask,                                         /* Task function. */
    "displayStateTask",                                         /* String with name of task. */
    5000,                                                       /* Stack size in bytes. */
    NULL,                                                       /* Parameter passed as input of the task */
    p_displayStateTask,                                         /* Priority of the task. */
    &h_displayStateTask,                                        /* Task handle to keep track of created task */
    0                                                           /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: displayStateTask");          
  delay(tc_objCreate_delay);

  //_________________________CONTROL_TASKS:

  /******************************************************************************************/
  /* Controls the Fan. Calls shtReadTask() & toggleFan(bool state).                         */
  /* Can change the currentState between OFF <--> COOLING                                   */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_fanControlTask,                                           /* Task function. */
    "fanControlTask",                                           /* String with name of task. */
    5000,                                                       /* Stack size in bytes. */
    NULL,                                                       /* Parameter passed as input of the task */
    p_fanControlTask,                                           /* Priority of the task. */
    &h_fanControlTask,                                          /* Task handle to keep track of created task */
    0                                                           /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: fanControlTask");            
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Controls the Heating. Calls shtReadTask() & toggleHeat(bool state).                    */
  /* Can change the currentState between OFF <--> HEATING                                   */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_heatControlTask,                                          /* Task function. */
    "heatControlTask",                                          /* String with name of task. */
    5000,                                                       /* Stack size in bytes. */
    NULL,                                                       /* Parameter passed as input of the task */
    p_heatControlTask,                                          /* Priority of the task. */
    &h_heatControlTask,                                         /* Task handle to keep track of created task */
    0                                                           /* pin task to core 0 */
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
    T_scanI2CTask,                                               /* Task function. */
    "scanI2CTask",                                               /* String with name of task. */
    5000,                                                        /* Stack size in bytes. */
    NULL,                                                        /* Parameter passed as input of the task */
    p_scanI2CTask,                                               /* Priority of the task. */
    &h_scanI2CTask,                                              /* Task handle to keep track of created task */
    1                                                            /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: scanI2CTask");               
  delay(tc_objCreate_delay);

  //__________________________COM_TASKS:

  /******************************************************************************************/
  /* Task responsable for connecting to WiFi Networks                                       */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_setupWifiTask,                                             /* Task function. */
    "setupWifiTask",                                             /* String with name of task. */
    5000,                                                        /* Stack size in bytes. */
    NULL,                                                        /* Parameter passed as input of the task */
    p_setupWiFiTask,                                             /* Priority of the task. */
    &h_setupWiFiTask,                                            /* Task handle to keep track of created task */
    1                                                            /* pin task to core 1 */
  );
  Serial.println("  >> Created Task: setupWifiTask");            
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Task responsable for MQTT communication: reconnecting to server,                       */
  /* publishing data, making decisions based on received data and sending updates to system */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_MQTT_CommunciationTask,                                    /* Task function. */
    "MQTT_CommunciationTask",                                    /* String with name of task. */
    5000,                                                        /* Stack size in bytes. */
    NULL,                                                        /* Parameter passed as input of the task */
    p_MQTT_CommunciationTask,                                    /* Priority of the task. */
    &h_MQTT_CommunciationTask,                                   /* Task handle to keep track of created task */
    1                                                            /* pin task to core 1 */
  );
  Serial.println("  >> Created Task: MQTT_CommunciationTask");    
  delay(tc_objCreate_delay);

  /******************************************************************************************/
  /* Task responsable for MQTT communication: publishing data                               */
  /******************************************************************************************/
  xTaskCreatePinnedToCore(
    T_MQTT_PublishTask,                                          /* Task function. */
    "T_MQTT_PublishTask",                                        /* String with name of task. */
    5000,                                                        /* Stack size in bytes. */
    NULL,                                                        /* Parameter passed as input of the task */
    p_MQTT_publishDataTask,                                      /* Priority of the task. */
    &h_MQTT_publishDataTask,                                     /* Task handle to keep track of created task */
    1                                                            /* pin task to core 1 */
  );
  Serial.println("  >> Created Task: T_MQTT_PublishTask");    
  delay(tc_objCreate_delay);

/****************************************************************************************************************************/

  Serial.println("<**> TASKS CREATED <**>");
  delay(tc_moduleSetup_delay);

}

/****************************************************************************************************************************/

#endif                                                           /* ESP_TASKS */


