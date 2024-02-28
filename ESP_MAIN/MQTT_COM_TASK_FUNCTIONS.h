#include "esp32-hal.h"
/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: MQTT_COM_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de comunicarea prin standardrul de comuncatie MQTT

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:   -> T_MQTT_CommunciationTask(void* parameter) 
                                                             -> functia de comunicare:            -> MQTT_Communciation()
                                                             -> functia de reconectare:           -> reconnect() 
                                                             -> functia de callback:              -> callback(char* topic, byte* message, unsigned int length) 
                                                             -> functia de publish:               -> publishData()  
                                                             */

#ifndef MQTT_COM_TASK_FUNCTIONS
#define MQTT_COM_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/
#include <PubSubClient.h>                                                                 /* MQTT Communication Protocol library */

/*__MODULES__*/
#include "DEFINES.h"                                                                      /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void MQTT_Communciation();                                                                /* Function responsable for MQTT communication: reconnecting to server, publishing data, making decisions based on received data and sending updates to the system */
void reconnect();                                                                         /* Subroutine for MQTT Communucation responsable for reconnecting to MQTT Server in case of dissconnecting  */
void publishData();                                                                       /* Subroutine for MQTT Communication responsable for publishing data to subscribed MQTT topics  */
void callback(char* topic, byte* message, unsigned int length);                           /* Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */


/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_MQTT_CommunciationTask(void* parameter)                                            /* Main function of the Task */
{
  for(;;)
  {
    char* taskName = "MQTT_CommunciationTask";

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/ 

    dataLog(taskName, p_MQTT_CommunciationTask);                                           /* Task info */ 


    MQTT_Communciation();                                                                 /* Function responsable for MQTT communication */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                        /* Delay the task */
  }

}

//__TASK_SUB-ROUTINES__


/****************************************************************************************************************************/
/*____________________________________________MQTT_COMMUNICATION_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void MQTT_Communciation()                                                                 /* Function responsable for MQTT communication: reconnecting to server, publishing data, making decisions based on received data and sending updates to the system */
{
  
  if (!client.connected())                                                                /* If not connected to MQTT Server, try to reconnect */
  {
    reconnect();
  }
  else if(client.connected())
  {
    client.loop();                                                                        /* Client loop : responsable for MQTT communication between the publishers and the subscribers, contains the callback function */  
    publishData();                                                                        /* Subroutine for MQTT Communication responsable for publishing data to subscribed MQTT topics  */        
    long now = millis();                                                                  

    if (now - lastMsg > 5000) 
    {
      lastMsg = now;                                                                      /* Last message received */
    }

  }

}

/****************************************************************************************************************************/
/*_________________________________________________MQTT_CALLBACK_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void callback(char* topic, byte* message, unsigned int length)                            /* Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */
{
  Serial.print("<MQTT> Callback: ");
  Serial.print("--> Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;                                                                     

  for( int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];                                                      /* Msg is constructed based on the information that comes via MQTT topic */
  }
  Serial.println();

  /***************************************************************/
  /*________Decisions_according_to_msg_on_specific_topics________*/
  /***************************************************************/
  if (String(topic) == "esp32/no.1/outputRed")                                            /* Light controls */      
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
  else if (String(topic) == "esp32/no.1/Reset")                                           /* ESP Reset */
  {
    if (messageTemp == "reset_esp1")
    {
      Serial.print("--> Reset the module !");
      delay(tc_ESPReset_delay);
      ESP.restart();
    }
  }
  else if (String(topic) == "esp32/no.1/temperatureControl")                              /* Assigning the value to v_temperatureThreshold */
  {
    float v_receivedThreshold = messageTemp.toFloat();                                    /* Msg is received as a string, convert to float */
    v_temperatureThreshold = v_receivedThreshold;                         
  }

}

/****************************************************************************************************************************/
/*________________________________________________MQTT_RECONNECT_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void reconnect()                                                                          /* Subroutine for MQTT Communucation responsable for reconnecting to MQTT Server in case of dissconnecting  */
{
  while (!client.connected())                                                             /* Loop until we're reconnected */
  {
    // Serial.print(" <MQTT> Reconnect:");
    if (client.connect(ID))                                                               /* Attempt to connect */
    {
      Serial.print(" <MQTT> Reconnect: ");
      Serial.println("connected");
      digitalWrite(pin_ledBuiltInPin, HIGH);                                              /* Signals connection succsessful to MQTT Server

      /****************************************************/
      /* Subscribe_to_MQTT_topics_(UI --> SERVER --> ESP) */
      /****************************************************/
      client.subscribe("esp32/no.1/outputRed");
      client.subscribe("esp32/no.1/Reset");
      client.subscribe("esp32/no.1/temperatureControl");

    }
    else
    {
      Serial.print(" <MQTT> Reconnect: ");
      Serial.print(" failed, rc=");
      Serial.print(client.state());                                                       /* MQTT rc (Response Codes) for connection and client status */
      Serial.println(" try again in 5 seconds!");

      delay(tc_mqttReconnect_delay);                                                      /* Wait 5 seconds before retrying */

    }
  }
}

/****************************************************************************************************************************/
/*__________________________________________________MQTT_PUBLISH_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void publishData()                                                                        /* Subroutine for MQTT Communication responsable for publishing data to subscribed MQTT topics  */
{
  Serial.println("<MQTT> Publish Data: ");

  char roomTempStr[8];                                                                   /* Convert to char Temperarture */
  dtostrf(v_temperatureValue,1,2,roomTempStr);
  Serial.print("--> Temperature converted to string: ");
  Serial.println(roomTempStr);
  client.publish("esp32/no.1/roomTemp",roomTempStr);                                     /* Publish to Python Program of the MQTT Server: roomTempStr (Temeprature) via topic: "esp32/no.1/roomTemp" */

  char airHumStr[8];                                                                     /* Convert to char Humidity */
  dtostrf(v_humidityValue,1,2,airHumStr);
  Serial.print("--> Humidity converted to string: ");
  Serial.println(airHumStr);
  client.publish("esp32/no.1/airHum",airHumStr);                                         /* Publish to Python Program of the MQTT Server: airHumStr (Air Humidity %)via topic: "esp32/no.1/airHum"*/
 
  Serial.print("--> State converted to string: ");
  Serial.println(stateToString(currentState));
  client.publish("esp32/no.1/sysState",(stateToString(currentState)));                   /* Publish to Python Program of the MQTT Server: stateToString(currentState) (Climate control state)via topic: "esp32/no.1/sysState" */
}
/****************************************************************************************************************************/

#endif                                                                                    /* _TASK_FUNCTIONS */


