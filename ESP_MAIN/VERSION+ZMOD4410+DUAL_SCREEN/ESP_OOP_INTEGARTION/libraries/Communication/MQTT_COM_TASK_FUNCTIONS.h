// #include "esp32-hal.h"
/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: MQTT_COM_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de comunicarea prin standardrul de comuncatie MQTT

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:   -> T_MQTT_CommunciationTask(void* parameter)
                                                             -> functia de comunicare:            -> MQTT_Communciation()
                                                             -> functia de reconectare:           -> reconnect()
                                                             -> functia de callback:              -> callback(char* topic, byte* message, unsigned int length)
                                                             -> functia de publish:               -> publishData() ; mutata in modul separat
                                                             */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef MQTT_COM_TASK_DEFINITION
#define MQTT_COM_TASK_DEFINITION

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
class MQTTCommunication
{
private:
  static void reconnect(); /* Subroutine for MQTT Communucation responsable for reconnecting to MQTT Server in case of dissconnecting  */
public:
  MQTTCommunication();
  static void MQTT_Communciation();                                      /* Function responsable for MQTT communication: reconnecting to server, publishing data, making decisions based on received data and sending updates to the system */
  static void callback(char *topic, byte *message, unsigned int length); /* Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */
};

extern MQTTCommunication mqttCOM;
/****************************************************************************************************************************/
#endif /* MQTT_COM_TASK_DEFINITION */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef MQTT_COM_TASK_FUNCTIONS
#define MQTT_COM_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_MQTT_CommunciationTask(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "MQTT_CommunciationTask"; /* Task name for dataLog function */

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_MQTT_CommunciationTask); /* Task info */

    mqttCOM.MQTT_Communciation(); /* Function responsable for MQTT communication */

    /*____________________________________*/

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
  }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*____________________________________________MQTT_COMMUNICATION_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void MQTTCommunication::MQTT_Communciation() /* Function responsable for MQTT communication: reconnecting to server, publishing data, making decisions based on received data and sending updates to the system */
{

  if (!client.connected()) /* If not connected to MQTT Server, try to reconnect */
  {
    mqttCOM.reconnect();
    mqttStatus = "Disconnected";
    
  }
  else if (client.connected())
  {
    client.loop(); /* Client loop : responsable for MQTT communication between the publishers and the subscribers, contains the callback function */
    // publishData();                                                                        /* Subroutine for MQTT Communication responsable for publishing data to subscribed MQTT topics  */

    mqttStatus = "Connected";

    long now = millis();
    if (now - lastMsg > 5000)
    {
      lastMsg = now; /* Last message received */
    }
  }
}

/****************************************************************************************************************************/
/*_________________________________________________MQTT_CALLBACK_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void MQTTCommunication::callback(char *topic, byte *message, unsigned int length) /* Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */
{
  Serial.print("<MQTT> Callback: ");
  Serial.print("--> Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i]; /* Msg is constructed based on the information that comes via MQTT topic */
  }
  Serial.println();

  /***************************************************************/
  /*________Decisions_according_to_msg_on_specific_topics________*/
  /***************************************************************/
  if (String(topic) == "esp32/no.1/outputRed") /* Light controls */
  {
    Serial.print("--> Changing output to ");
    if (messageTemp == "RedLEDon")
    {
      Serial.println("-->Red LED on");
      digitalWrite(pin_ledRedPin, HIGH);
    }
    else if (messageTemp == "RedLEDoff")
    {
      Serial.println("--> Red LED off");
      digitalWrite(pin_ledRedPin, LOW);
    }
  }
  else if (String(topic) == "esp32/no.1/Reset") /* ESP Reset */
  {
    if (messageTemp == "reset_esp1")
    {
      Serial.print("--> Reset the module !");
      delay(tc_ESPReset_delay);
      ESP.restart(); /* Restart the ESP module */
    }
  }
  else if (String(topic) == "esp32/no.1/temperatureControl") /* Temperature control */
  {
    float v_receivedThreshold = messageTemp.toFloat();         /* Msg is received as a string, convert to float */
    receivedData.v_temperatureThreshold = v_receivedThreshold; /* Assign the value to v_temperatureThreshold */
  }
  else if (String(topic) == "esp32/no.1/getOutsideTemp") /* Get the outisde temperature */
  {
    float v_receivedOutsideTemperature = messageTemp.toFloat(); /* Msg is received as a string, convert to float */
    receivedData.v_outsideTemp = v_receivedOutsideTemperature;  /* Assign the value to v_outsideTemperature */
  }
}

/****************************************************************************************************************************/
/*________________________________________________MQTT_RECONNECT_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void MQTTCommunication::reconnect() /* Subroutine for MQTT Communucation responsable for reconnecting to MQTT Server in case of dissconnecting  */
{
  while (!client.connected()) /* Loop until we're reconnected */
  {
    // Serial.print(" <MQTT> Reconnect:");
    if (client.connect(ID)) /* Attempt to connect */
    {
      Serial.print(" <MQTT> Reconnect: ");
      Serial.println("connected");
      delay(500);
      digitalWrite(pin_ledBuiltInPin, HIGH); /* Signals connection succsessful to MQTT Server

      /****************************************************/
      /* Subscribe_to_MQTT_topics_(UI --> SERVER --> ESP) */
      /****************************************************/
      client.subscribe("esp32/no.1/outputRed");
      client.subscribe("esp32/no.1/Reset");
      client.subscribe("esp32/no.1/temperatureControl");
      client.subscribe("esp32/no.1/getOutsideTemp");
      client.subscribe("esp32/no.1/airPressure");
      client.subscribe("esp32/no.1/IAQ");
      client.subscribe("esp32/no.1/eCO2");
      client.subscribe("esp32/no.1/TVOC");
      client.subscribe("esp32/no.1/EtOH");
    }
    else
    {
      Serial.print(" <MQTT> Reconnect: ");
      Serial.print(" failed, rc=");
      Serial.print(client.state());         /* MQTT rc (Response Codes) for connection and client status */
      digitalWrite(pin_ledBuiltInPin, LOW); /* Signals connection lost to MQTT Server */
      Serial.println(" try again in 5 seconds!");

      delay(tc_mqttReconnect_delay); /* Wait 5 seconds before retrying */
    }
  }
}

/****************************************************************************************************************************/
#endif /* MQTT_COM_TASK_FUNCTIONS */
