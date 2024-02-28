/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SETUP_WIFI_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de conectarea la retele WiFi

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                     -> T_setupWifiTask(void* parameter)
                                                             -> funtia care se ocupa de conectarea la retele WiFi:  -> setupWiFi() 
                                                             */

#ifndef SETUP_WIFI_TASK_FUNCTIONS
#define SETUP_WIFI_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/
#include <WiFi.h>                                                                         /* WiFi communication library */

/*__MODULES__*/
#include "DEFINES.h"                                                                      /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void setupWiFi();                                                                         /* Function responsable for connecting to WiFi Networks */


/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_setupWifiTask(void* parameter)                                                     /* Main function of the Task */
{
  for(;;)
  {
    char* taskName = "setupWifiTask";

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/ 

    dataLog(taskName, p_setupWiFiTask);                                                   /* Task info */ 

    setupWiFi();                                                                          /* Function responsable for connecting to WiFi Networks */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                        /* Delay the task */
  }

}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*__________________________________________________SETUP_WiFI_FUNCTION_____________________________________________________*/
/****************************************************************************************************************************/
void setupWiFi()                                                                          /* Function responsable for connecting to WiFi Networks */
{


  /* Task Code */
  // Serial.print("--> Connecting to: ");
  // Serial.print(ssid);

  WiFi.begin(ssid, password);                                                             /* Begin WiFi Communication */

  while (WiFi.status() != WL_CONNECTED)                                                   /* Check if ESP is not connected to WiFi networks */
  {
    Serial.print("<WiFi> Connecting to: ");
    Serial.print(ssid);

    delay(tc_wifiSetupScan_delay);
    Serial.print(".");                                                                    /* To show the time passed until connection succsessful */
  }

  if (WiFi.status() == WL_CONNECTED)                                                    
  {
    Serial.print("<WiFi> Connecting to: ");
    Serial.print(ssid);

    Serial.print(" --> WiFi connected");
    Serial.print(" --> IP address: ");
    Serial.println(WiFi.localIP());
    vTaskSuspend(NULL);                                                                   /* Suspend the task if connection is succsessful */

  }
    
}

/****************************************************************************************************************************/

#endif                                                                                    /* SETUP_WIFI_TASK_FUNCTIONS */


