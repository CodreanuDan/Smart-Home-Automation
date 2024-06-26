/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SETUP_WIFI_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de conectarea la retele WiFi

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                     -> T_setupWifiTask(void* parameter)
                                                             -> funtia care se ocupa de conectarea la retele WiFi:  -> setupWiFi()
                                                             */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SETUP_WIFI_TASK_DEFINITIONS
#define SETUP_WIFI_TASK_DEFINITIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/
#include <WiFi.h> /* WiFi communication library */

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
class SetupWIFI
{
public:
  SetupWIFI();
  static void setupWiFi(); /* Function responsable for connecting to WiFi Networks */
};

extern SetupWIFI setupWifiConn;

/****************************************************************************************************************************/
#endif /* SETUP_WIFI_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SETUP_WIFI_TASK_FUNCTIONS
#define SETUP_WIFI_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_setupWifiTask(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "setupWifiTask"; /* Task name for dataLog function */

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_setupWiFiTask); /* Task info */

    setupWifiConn.setupWiFi(); /* Function responsable for connecting to WiFi Networks */

    /*____________________________________*/

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
  }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*__________________________________________________SETUP_WiFI_FUNCTION_____________________________________________________*/
/****************************************************************************************************************************/
void SetupWIFI::setupWiFi() /* Function responsable for connecting to WiFi Networks */
{

  /* Task Code */
  // Serial.print("--> Connecting to: ");
  // Serial.print(ssid);

  WiFi.begin(ssid, password); /* Begin WiFi Communication */

  while (WiFi.status() != WL_CONNECTED) /* Check if ESP is not connected to WiFi networks */
  {
    Serial.print("<WiFi> Connecting to: ");
    Serial.print(ssid);

    wifiStatus = String(WiFi.status()); /* Get the WiFi status */

    delay(tc_wifiSetupScan_delay);
    Serial.print("."); /* To show the time passed until connection succsessful */
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("<WiFi> Connecting to: ");
    Serial.print(ssid);

    wifiStatus = String(WiFi.status()); /* Get the WiFi status */

    Serial.print(" --> WiFi connected");
    Serial.print(" --> IP address: ");
    Serial.println(WiFi.localIP());
    
    /************************_NTP_SERVER_************************/
    configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER); /* Set the time zone and NTP server */
    /***********************************************************/

    delay(1000);
    vTaskSuspend(NULL); /* Suspend the task if connection is succsessful */

  }
}

/****************************************************************************************************************************/
#endif /* SETUP_WIFI_TASK_FUNCTIONS */
