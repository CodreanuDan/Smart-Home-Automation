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
#include <ArduinoBLE.h>       // BLE communication library

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
class SetupWIFI
{
public:
  SetupWIFI();
  static void ble_wifi_config(); /* Function responsable for connecting to WiFi Networks trough BLE data */
  static void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);  /* Event handler for connection to WiFi AP */
  static void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);  /* Event handler for obtaining IP address from WiFi AP */
  static void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info); /* Event handler for disconnection from WiFi AP */
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
//   for (;;)
//   {
    char *taskName = "setupWifiTask"; /* Task name for dataLog function */

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_setupWiFiTask); /* Task info */

    setupWifiConn.ble_wifi_config(); /* Function responsable for connecting to WiFi Networks */

    /*____________________________________*/

    // vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
//   }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*________________________________________WiFiStationConnected_EVENT_FUNCTION_______________________________________________*/
/****************************************************************************************************************************/
void SetupWIFI::WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)  /* Event handler for connection to WiFi AP */
{
  unsigned long eventTime = millis(); /* Get the current time in milliseconds */
  Serial.print("[WIFI_EVENT_CONNECT] Event on_connect active at: ");Serial.print(eventTime);Serial.println(" ms");
  Serial.println("[WIFI_EVENT_CONNECT] Connected to AP successfully, config time zone for NTP Server!");

  /************************_NTP_SERVER_************************/
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER); /* Set the time zone and NTP server */
  /***********************************************************/
  
  wifiStatus = String(WiFi.status()); /* Get the WiFi status */
//   digitalWrite(2,HIGH); /*Signal connection success*/
}

/****************************************************************************************************************************/
/*_____________________________________________WiFiGotIP_EVENT_FUNCTION_____________________________________________________*/
/****************************************************************************************************************************/
void SetupWIFI::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)  /* Event handler for obtaining IP address from WiFi AP */
{
  unsigned long eventTime = millis(); /* Get the current time in milliseconds */ 
  Serial.print("[WIFI_EVENT_IP_CONNACK] Event on_connect active at: ");Serial.print(eventTime);Serial.println(" ms");
  Serial.print("[WIFI_EVENT_IP_CONNACK] WiFi connected at IP ADDR:");Serial.println(WiFi.localIP());
}

/****************************************************************************************************************************/
/*______________________________________WiFiStationDisconnected_EVENT_FUNCTION______________________________________________*/
/****************************************************************************************************************************/
void SetupWIFI::WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) /* Event handler for disconnection from WiFi AP */
{
  unsigned long eventTime = millis(); /* Get the current time in milliseconds */
  Serial.print("[WIFI_EVENT_DISCONNECT] Event on_disconnect active at: ");Serial.print(eventTime);Serial.println(" ms");
  Serial.print("[WIFI_EVENT_DISCONNECT] Disconnected from WiFi AP Reason:");Serial.println(info.wifi_sta_disconnected.reason);
  wifiStatus = String(WiFi.status()); /* Get the WiFi status */
//   digitalWrite(2,LOW); /* Signal Connection loss*/
  
  if (h_setupWiFiTask != NULL) /* Resume BLETask to reconnect to wifi */
  {
    Serial.println("[WIFI_EVENT_DISCONNECT] *Resuming BLE Task");
    vTaskResume(h_setupWiFiTask); /* Resume BLETask */
  }

}

/****************************************************************************************************************************/
/*________________________________________________WIFI_NET_CONN_BLE_FUNCTION________________________________________________*/
/****************************************************************************************************************************/
void SetupWIFI::ble_wifi_config() /* Function responsable for configuring the BLE connection to WiFi */
{
  /* UUIDs for the service and characteristiC */ 
  const char* serviceUUID = "12345678-1234-1234-1234-123456789012";
  const char* characteristicUUID = "87654321-4321-4321-4321-210987654321";

  /*Create BLE Service*/ 
  BLEService myService(serviceUUID);

  /* Create BLE String Characteristic */ 
  BLEStringCharacteristic myCharacteristic(characteristicUUID, BLERead | BLEWrite, 40);

  /* Initialize BLE */ 
  if (!BLE.begin()) 
  {
    Serial.println("[BLE_CONN] *starting BLE failed!");
    while (1);
  }

  /* Set the local name for the BLE device */
  BLE.setLocalName("ESP32_BLE");
  BLE.setAdvertisedService(myService); /* Advertise the custom service */ 

  /* Add the characteristic to the service */
  myService.addCharacteristic(myCharacteristic);

  /* Add the service */
  BLE.addService(myService);

  /* Start advertising */
  BLE.advertise();
  Serial.println("[BLE_CONN] *BLE device is now advertising");

  while (true)
  {
    if(WiFi.status() != WL_CONNECTED) /* Check if Wifi is disconnected*/
    {

      wifiStatus = String(WiFi.status()); /* Get the WiFi status */
      BLEDevice central = BLE.central(); /* Wait for a BLE central */

      if (central)  /* If a central is connected to the peripheral */
      {
        Serial.print("[BLE_CONN] *Connected to central: ");
        Serial.println(central.address()); /* Print the central's BT address */

        while (central.connected())  /* While the central is still connected */
        {
          if (myCharacteristic.written())  /* If the central wrote to the characteristic */
          {
            String receivedValue = myCharacteristic.value(); /* Read the value of the characteristic */
            Serial.print("[BLE_CONN] *Received Value: ");
            Serial.println(receivedValue); /* Print the value to the serial monitor */

            int separatorIndex = receivedValue.indexOf(':'); /* Find the separator in the value */
            if (separatorIndex != -1) 
            {
              ssid = receivedValue.substring(0, separatorIndex); /* Parse the value */
              password = receivedValue.substring(separatorIndex + 1); 
              Serial.print("[BLE_CONN] *Parsed SSID: ");
              Serial.print(ssid);
              Serial.print(" | Parsed Password: ");
              Serial.println(password);

              /*********************_Connect_to_WiFi_**********************/	

              Serial.print("[WIFI_CONN] *Connecting to Wi-Fi");
              WiFi.begin(ssid.c_str(), password.c_str()); /* Connect to the Wi-Fi network */

              int attempts = 0; /* Wait for the Wi-Fi to connect */
              while (WiFi.status() != WL_CONNECTED && attempts < 20)  /* Wait for the Wi-Fi to connect */
              {
                delay(500);
                Serial.print(".");
                attempts++;
              }

              if (WiFi.status() == WL_CONNECTED)  /* If the Wi-Fi connected successfully */
              {
                Serial.print("[WIFI_CONN] <!><!><!>Connected to Wi-Fi IP ADDR:");
                Serial.print(WiFi.localIP()); /* Print the IP address */
                Serial.println("[WIFI_CONN] <!><!><!>");
                wifiStatus = String(WiFi.status()); /* Get the WiFi status */
                Serial.println("[WIFI_CONN] *Suspending BLE Task");
                vTaskSuspend(NULL); /* Suspend task when connected */ 
              } 
              else 
              {
                Serial.println("[WIFI_CONN] *Failed to connect to Wi-Fi");
              }

              /***************************************************************/
            } 
            else 
            {
              Serial.println("[BLE_CONN] *Invalid format. Expected ssid:password");
            }
          }
          delay(10);
        }
        Serial.print("[BLE_CONN] *Disconnected from central: ");
        BLE.disconnect(); /* Disconnect from the central */
        Serial.println(central.address()); /* Print the central's BT address */
      }
    }
    delay(10);
  }
}

/****************************************************************************************************************************/
#endif /* SETUP_WIFI_TASK_FUNCTIONS */
