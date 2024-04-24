/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: MQTT_PUB_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de trimiterea de date la serverul MQTT

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:   -> T_MQTT_PublishTask(void* parameter)
                                                             -> functia de comunicare:            -> MQTT_publishData()
                                                             */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef MQTT_PUB_TASK_DEFINITION
#define MQTT_PUB_TASK_DEFINITION

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/
#include <PubSubClient.h> /* MQTT Communication Protocol library */

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
class MQTTPublishing
{
public:
  MQTTPublishing();
  static void MQTT_publishData();     /* Function responsable for MQTT communication: publishing data to MQTT Server*/
  static void MQTT_publishSysState(); /* Function responsable for MQTT communication: publishing system state to MQTT Server*/
};

extern MQTTPublishing mqttPUB;
/****************************************************************************************************************************/
#endif /* MQTT_PUB_TASK_DEFINITION */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef MQTT_PUB_TASK_FUNCTIONS
#define MQTT_PUB_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_MQTT_PublishTask(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "T_MQTT_PublishTask"; /* Task name for dataLog function */

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_MQTT_CommunciationTask); /* Task info */

    mqttPUB.MQTT_publishData(); /* Function responsable for MQTT communication */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_MQTT_publishDataTask_delay)); /* Delay the task */
  }
}

void T_MQTT_publishSysState(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "T_MQTT_publishSysState"; /* Task name for dataLog function */

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_MQTT_CommunciationTask); /* Task info */

    mqttPUB.MQTT_publishSysState(); /* Function responsable for MQTT communication */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_MQTT_publishSysState_delay)); /* Delay the task */
  }
}

/****************************************************************************************************************************/
/*__________________________________________________MQTT_PUBLISH_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void MQTTPublishing::MQTT_publishData() /* Subroutine for MQTT Communication responsable for publishing data to subscribed MQTT topics  */
{
  Serial.println("<MQTT> Publish Data: ");

  static char lastFinalTempStr[8] = ""; /* Static variables for storing the last published values */
  static char lastFinalHumStr[8] = "";  /* Static variables for storing the last published values */
  static char lastPressStr[8] = "";     /* Static variables for storing the last published values */

  /*********************************************************************/
  /*_____________________Final_Unified_Sensor_Data_____________________*/
  /*********************************************************************/

  char finalTempStr[8];
  dtostrf(avgVal.v_final_temperatureValue, 1, 2, finalTempStr); /* Convert to char Final Temperature */
  if (strcmp(finalTempStr, lastFinalTempStr) != 0)
  {
    if (finalTempStr != 0)
    {
      client.publish("esp32/no.1/roomTemp", finalTempStr); /* Publish to Python Program of the MQTT Server: finalTempStr (Final Temperature) via topic: "esp32/no.1/roomTemp" */
      Serial.print("Published to MQTT Server: ");Serial.print("esp32/no.1/roomTemp");Serial.print(" : ");Serial.println(finalTempStr);
      strcpy(lastFinalTempStr, finalTempStr);
    }
  }

  char finalHumStr[8];
  dtostrf(avgVal.v_final_humidityValue, 1, 2, finalHumStr); /* Convert to char Final Humidity */
  if (strcmp(finalHumStr, lastFinalHumStr) != 0)
  {
    if (finalHumStr != 0)
    {
      client.publish("esp32/no.1/airHum", finalHumStr); /* Publish to Python Program of the MQTT Server: finalHumStr (Final Humidity) via topic: "esp32/no.1/roomHum" */
      Serial.print("Published to MQTT Server: ");Serial.print("esp32/no.1/roomHum");Serial.print(" : ");Serial.println(finalHumStr);
      strcpy(lastFinalHumStr, finalHumStr);
    }
  }

  /*********************************************************************/
  /*_______________________Air_pressure_data___________________________*/
  /*********************************************************************/
  char PressStr[8];
  dtostrf(bmeData.v_bme_pressureValue, 1, 2, PressStr); /* Convert to char Final Pressure */
  if (strcmp(PressStr, lastPressStr) != 0)
  {
    if (PressStr != 0)
    {
      client.publish("esp32/no.1/airPressure", PressStr); /* Publish to Python Program of the MQTT Server: PressStr (Final Pressure) via topic: "esp32/no.1/roomPress" */
      Serial.print("Published to MQTT Server: ");Serial.print("esp32/no.1/roomPress"); Serial.print(" : ");Serial.println(PressStr);
      strcpy(lastPressStr, PressStr);
    }
  }
}

/****************************************************************************************************************************/
/*________________________________________MQTT_PUBLISH_SYS_STATE_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void MQTTPublishing::MQTT_publishSysState() /* Subroutine for MQTT Communication responsable for publishing system state to subscribed MQTT topics  */
{
  /**********************************************************************/
  /*_______________________CLIMATE_CONTROL_DATA_________________________*/
  /**********************************************************************/
  Serial.print("--> State converted to string: ");
  Serial.println(stateToString(currentState));
  const char *sysStateStr = stateToString(currentState); /* Convert to char Climate control state */
  client.publish("esp32/no.1/sysState", sysStateStr);    /* Publish to Python Program of the MQTT Server: sysStateStr (Climate control state)via topic: "esp32/no.1/sysState" */
  Serial.print("Published to MQTT Server: ");Serial.print("esp32/no.1/sysState");Serial.print(" : ");Serial.println(sysStateStr);
}

/****************************************************************************************************************************/
#endif /* MQTT_PUB_TASK_FUNCTIONS */
