/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: DEFINES.h */

/*   ---> Modulul contine variabilele, constantele si define-urile programului si anumite functii ajutatoare

                                                            */

#ifndef DEFINES
#define DEFINES

/*###################################################################################################################################################################*/
/*__________________________________________________________________DEFINES_&_DECLARATIONS:__________________________________________________________________________*/
/*###################################################################################################################################################################*/



/****************************************************************************************************************************/
/*________________________________________________________SHT_21____________________________________________________________*/
/****************************************************************************************************************************/
SHT21 sht;                                                      /* Declararea unui obiect de tip SHT21 numit sht */ 

//__SHT_VALUES__
float v_temperatureValue;                                       /* SHT temp (C*)   */
float v_humidityValue;                                          /* SHT RH (%)      */


/****************************************************************************************************************************/
/*______________________________________________________WIFI_LIB____________________________________________________________*/
/****************************************************************************************************************************/
WiFiClient espClient;                                           /* Declararea unui obiect de tip WiFiClient numit espClient */


/****************************************************************************************************************************/
/*________________________________________________________MQTT______________________________________________________________*/
/****************************************************************************************************************************/

//__________________MQTT_OBJECTS:
PubSubClient client(espClient);                                 /* Declararea unui obiect de tip PubSubClient numit client care primește ca argument un obiect WiFiClient existent (espClient) */

//_____________MQTT_MESSAGE_DATA:
long lastMsg    = 0;                                            /* */                                     
int  value      = 0;                                            /* */
char msg[50];                                                   /* Size of message */

//______________MQTT_SERVER_DATA:

//__Device_Identifier:
#define ID "ESP32/Module_1"

//_Wi-Fi_Network_Name(SSID):
// #define ssid "CDIDC"
#define ssid "TP-Link_47D4"

//_Wi-Fi_Network_Password:
// #define password "SkodaOctavia2005"
#define password "29393145"

//_MQTT_Server_Adress:
// #define mqtt_server "192.168.0.24"                               /* MQTT Server --> BACAU  */
#define mqtt_server "192.168.1.193"                             /* MQTT Server --> IASI  */


/****************************************************************************************************************************/
/*__________________________________________________CLIMATE_CONTROL_________________________________________________________*/
/****************************************************************************************************************************/

//_____________STATES:
enum State                                                      /* enum of the states of the system    */
{
  OFF,                                                       /* OFF     :   Fan & Heating OFF       */
  COOLING,                                                   /* COOLING :   Fan ON & Heating OFF    */
  HEATING                                                    /* HEATING :   Heating ON & Fan OFF    */
};

//_______INITIAL_STATE:
State currentState = OFF;                                       /* Initial state at system start       */

//_____STATE_TO_STRING:
const char* stateToString(State state) 
{
  switch (state) 
  {
    case OFF:                                                /* OFF     : 0  */
      return "OFF";
    case COOLING:                                            /* COOLING : 1  */
      return "COOLING";
    case HEATING:                                            /* HEATING : 2  */
      return "HEATING";
    default:
      return "UNKNOWN";
  }
}

//__________THERSHOLDS:
float v_temperatureThreshold         = 0;                       /* Takes updated values from MQTT Topic responsable for temp. control (callback function from MQTT_COM_TASK_FUNCTIONS.h )    */
float v_humidityThreshold            = 70;                      /* It will be automated (remote control  ---> suggestion via fuzzy logic ?)                                                  */

//_RELAY_CONTROL_FLAGS:
bool f_fanControlTask  = false;                                 /*  Flag for Fan  Control - flag false --> true: flag ; decision -->  flag false  */
bool f_heatControlTask = false;                                 /*  Flag for Heat Control - flag false --> true: flag ; decision -->  flag false  */

/****************************************************************************************************************************/
/*____________________________________________________VARIABILES____________________________________________________________*/
/****************************************************************************************************************************/
/* const declaration: for saftey reasons + compiling is done quicker in this way (read only vars, cannot be written ?) */
/* #define declarations for the data types that accept this, task priority and handlers not                            */


/****************************************************************************************************************************/
/*_____________________________________________________PINS_________________________________________________________________*/
/****************************************************************************************************************************/

//________________INPUT_PINS: (will be added in the future)
/* FOR I2C BUS: --> D21: SDA - Serial data
                --> D22: SCL - Serial clock
                (ESP 32 Pinout diagram)     */

//_______________OUTPUT_PINS:
#define pin_heatRelayPin        16                              /* Heat relay  pin  (toggle heat on/off)                               */
#define pin_fanRelayPin         17                              /* Fan relay   pin  (toggle fan on/off)                                */

#define pin_ledBuiltInPin        2                              /* LedBuiltIn  pin  (signal if connection to MQTT Server is realized)  */   
#define pin_ledRedPin            4                              /* Pin for led(in role of lights ON/OFF)                               */

/****************************************************************************************************************************/
/*____________________________________________TASK_DATA_LOG_FUNCTION________________________________________________________*/
/****************************************************************************************************************************/

void dataLog(char* taskName, int taskPrio)                      /* Function that displays the data of the task */
{
  Serial.print("[#] ");
  Serial.print(taskName);                                       /* Task name */
  Serial.print(": ");

  Serial.print(" prio: ");
  Serial.print(taskPrio);                                       /* Task priority */

  Serial.print(" Core: ");
  Serial.print(xPortGetCoreID());                               /* Core where the task in running */

  Serial.print(" | ");
  // Serial.println(" -> Running OK ");

}

/****************************************************************************************************************************/
/*_______________________________________________TASK_PRIORITIES____________________________________________________________*/
/****************************************************************************************************************************/

// __TASK_PRIORITY__
// --> Highest_prio : lower number value, lowest prio: bigger number value

//_Core_0_Tasks_prio:

//__Sensor_Tasks:
const int p_shtReadTask              = 10;                      /*  >> shtReadTask priority: MEDIUM          */  
const int p_mqReadTask               = 10;                      /*  >> mqReadTask priority: MEDIUM           */  

//_Display_Tasks:
const int p_displayStateTask         = 20;                      /*  >> displayStateTask priority: MEDIUM     */
const int p_shtDisplayTask           = 20;                      /*  >> shtDisplayTask priority: MEDIUM       */

//_Control_Tasks:
const int p_fanControlTask           = 25;                      /*  >> fanControlTask priority: MEDIUM       */
const int p_heatControlTask          = 25;                      /*  >> heatControlTask priority: MEDIUM      */

//_Core_1_Tasks_prio:

//____Diag_Tasks:
const int p_scanI2CTask              = 1;                       /*  >> scanI2CTask priority: HIGH            */

//_____Com_Tasks:
const int p_setupWiFiTask            = 2;                       /*  >> setupWiFiTask priority: HIGH          */
const int p_MQTT_CommunciationTask   = 5;                       /*  >> MQTT_CommunciationTask priority: HIGH */

//__TIME_CONSTARINS__(ms):

#define tc_moduleSetup_delay         50                         /* Delay set between each part of the setup modules                                                  */
#define tc_objCreate_delay           25                         /* Delay set between initialization of each object part of a module in setup                         */
#define tc_taskFunc_delay            25                         /* Short delay before and after task routine executes                                                */
#define tc_quickTask_delay           1000                       /* Delay for short tasks (ex: display tasks etc.)                                                    */
#define tc_measureTask_delay         1000                       /* Delay for sensor tasks                                                                            */
#define tc_controlTask_delay         1000                       /* Delay for control tasks (ex: relay control tasks etc.)                                            */
#define tc_wifiSetupScan_delay       500                        /* Delay for WiFi setup - set the time between pings for trying to connect to WiFi networks          */
#define tc_mqttReconnect_delay       5000                       /* Delay for MQTT Server reconnect - Wait 5 seconds before retrying                                  */
#define tc_ESPReset_delay            1000                       /* Delay for ESP restart                                                                             */ 


/****************************************************************************************************************************/
/*______________________________________________ TASK_HANDLERS______________________________________________________________*/
/****************************************************************************************************************************/

//_Core_0_Task_handlers:

//__Sensor_Tasks:
TaskHandle_t                h_shtReadTask;                      /* >> shtReadTask handler            */
TaskHandle_t                h_mqReadTask;                       /* >> shtReadTask handler            */

//_Display_Tasks:
TaskHandle_t                h_displayStateTask;                 /* >> displayStateTask handler       */
TaskHandle_t                h_shtDisplayTask;                   /* >> shtDisplayTask handler         */

//_Control_Tasks:
TaskHandle_t                h_fanControlTask;                   /* >> fanControlTask handler         */
TaskHandle_t                h_heatControlTask;                  /* >> heatControlTask handler        */


//_Core_1_Task_handlers:

//____Diag_Tasks:
TaskHandle_t                h_scanI2CTask;                      /* >> scanI2CTask handler            */

//_____Com_Tasks:
TaskHandle_t                h_setupWiFiTask;                    /* >> setupWiFiTask handler          */
TaskHandle_t                h_MQTT_CommunciationTask;           /* >> MQTT_CommunciationTask handler */

/****************************************************************************************************************************/

#endif                                                                                    /* DEFINES */


