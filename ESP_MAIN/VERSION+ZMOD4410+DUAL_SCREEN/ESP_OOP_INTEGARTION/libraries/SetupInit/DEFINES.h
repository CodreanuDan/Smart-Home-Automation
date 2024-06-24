/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: DEFINES.h */

/*   ---> Modulul contine variabilele, constantele si define-urile programului si anumite functii ajutatoare

                                                            */

#ifndef DEFINES
#define DEFINES

/*##########################################################################################################################*/
/*|_______________________________________________DEFINES_&_DECLARATIONS:__________________________________________________|*/
/*##########################################################################################################################*/

/*##########################################################################################################################*/
/*|______________________________________________________SENSORS:__________________________________________________________|*/
/*##########################################################################################################################*/

/****************************************************************************************************************************/
/*__________________________________________________SHT_21_&_BME_680________________________________________________________*/
/****************************************************************************************************************************/
/* SHT 21 PINS: --> D21: I2C SDA - Serial data
                --> D22: I2C SCL - Serial clock
                (ESP 32 Pinout diagram - I2C bus pins           */

#define SEALEVELPRESSURE_HPA (1013.25)

struct ShtData /* SHT 21 data structure */
{
  float v_sht_temperatureValue; /* SHT 21 measured temperature */
  float v_sht_humidityValue;    /* SHT 21 measured humidity */
};
ShtData shtData; /* SHT 21 data structure object */

struct BmeData /* BME 280 data structure */
{
  float v_bme_temperatureValue; /* BME 280 measured temperature */
  float v_bme_pressureValue;    /* BME 280 measured pressure */
  float v_bme_humidityValue;    /* BME 280 measured humidity */
  float v_bme_gasResValue;      /* BME 280 measured gas resistance */
  float v_bme_ReadAltValue;     /* BME 280 measured altitude */
};
BmeData bmeData;

struct AvgVal /* Averaged values structure */
{
  float v_final_temperatureValue; /* Final temperature value */
  float v_final_humidityValue;    /* Final humidity value */
};
AvgVal avgVal; /* Averaged values structure object */

bool shtReadFlag; /* Flag for SHT 21 read task */
bool bmeReadFlag; /* Flag for BME 280 read task */

/****************************************************************************************************************************/
/*_____________________________________________________ZMOD_4410____________________________________________________________*/
/****************************************************************************************************************************/
/* ZMOD data comes trough UART 2 */
/* ZMOD 4410 PINS: --> RX2: UART 2 RX pin - 16
                  --> TX2: UART 2 TX pin - 17
                  (ESP 32 Pinout diagram - UART 2 pins) */

struct IaqData /* IAQ data structure */
{
  float v_IAQ;  /* IAQ value */
  float v_TVOC; /* TVOC value */
  float v_eCO2; /* eCO2 value */
  float v_EtOH; /* EtOH value */
  char s_sensorStat[20];/* H2 value */
};
IaqData iaqData; /* IAQ data structure object */

/*##########################################################################################################################*/
/*|________________________________________________________COM:____________________________________________________________|*/
/*##########################################################################################################################*/

/****************************************************************************************************************************/
/*____________________________________________________UART_2_TX2_RX2________________________________________________________*/
/****************************************************************************************************************************/
#define RX2_PIN 16 /* RX pin on ESP32 UART 2  */
#define TX2_PIN 17 /* TX pin on ESP32 UART 2  */

/****************************************************************************************************************************/
/*_______________________________________________________I2C_BUS____________________________________________________________*/
/****************************************************************************************************************************/
/* I2C_BUS_PINS: --> D21: I2C SDA - Serial data
                 --> D22: I2C SCL - Serial clock
               (ESP 32 Pinout diagram)     */

/*********_Second_I2C_for_OLED_Display_SSD1315_**********/
#define SDA_PIN_OLED 18 /* SDA pin on ESP32 I2C bus for OLED_Display_SSD1315 */
#define SCL_PIN_OLED 19 /* SCL pin on ESP32 I2C bus for OLED_Display_SSD1315 */

/****************************************************************************************************************************/
/*______________________________________________________WIFI_LIB____________________________________________________________*/
/****************************************************************************************************************************/
WiFiClient espClient; /* Declararea unui obiect de tip WiFiClient numit espClient */

//_Wi-Fi_Network_Name(SSID):
#define ssid "CDIDC"
// #define ssid "TP-Link_47D4"
// #define ssid "E204"

//_Wi-Fi_Network_Password:
#define password "SkodaOctavia2005"
// #define password "29393145"
// #define password "passwlane204ieeia"

/*********_Wifi_Connection_Flag_********/
String wifiStatus; /* Flag for WiFi connection status */

/****************************************************************************************************************************/
/*___________________________________________________NTP_SERVER_____________________________________________________________*/
/****************************************************************************************************************************/
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 3
#define UTC_OFFSET_DST 0

/****************************************************************************************************************************/
/*________________________________________________________MQTT______________________________________________________________*/
/****************************************************************************************************************************/

//__________________MQTT_OBJECTS:
PubSubClient client(espClient); /* Declararea unui obiect de tip PubSubClient numit client care primește ca argument un obiect WiFiClient existent (espClient) */

//_____________MQTT_MESSAGE_DATA:
long lastMsg = 0;  /* */
uint8_t value = 0; /* */
char msg[50];      /* Size of message */

//______________MQTT_SERVER_DATA:

//__Device_Identifier:
#define ID "ESP32/Module_1"

//_MQTT_Server_Adress:
#define mqtt_server "192.168.0.24"                               /* MQTT Server --> BACAU  */
// #define mqtt_server "192.168.1.193" /* MQTT Server --> IASI  */
// #define mqtt_server "192.168.0.152" /* MQTT Server --> E204  */

//__________THERSHOLDS_&_OUTSIDE_DATA:
struct ReceivedData /* Structure for the received data from the MQTT Server  */
{
  float v_outsideTemp = 0;          /* Takes updated values from MQTT Topic responsable for outside temperature (callback function from MQTT_COM_TASK_FUNCTIONS.h )    */
  float v_temperatureThreshold = 0; /* Takes updated values from MQTT Topic responsable for temp. control (callback function from MQTT_COM_TASK_FUNCTIONS.h )    */
  float v_humidityThreshold = 70;   /* It will be automated (remote control  ---> suggestion via fuzzy logic ?)                                                  */
};
ReceivedData receivedData; /* Received data structure object */

/*************_MQTT_Connection_Flag_*************/
String mqttStatus; /* Flag for MQTT connection status */

/*##########################################################################################################################*/
/*|____________________________________________________DISPLAY:____________________________________________________________|*/
/*##########################################################################################################################*/

/***************************************************************************************************************************/
/*|_____________________________________________________OLED_DISPLAY______________________________________________________|*/
/***************************************************************************************************************************/
/* OLED_DISPLAY_PINS: --> D21: I2C SDA - Serial data
                      --> D22: I2C SCL - Serial clock
                      (ESP 32 Pinout diagram)
*/

/**************CHANGE_DISPLAY_INFO_CONTROL_VARIABLES_*********************/
volatile uint8_t button_up_clicked = 0;   /* only perform action when button is clicked, and wait until another press */
volatile uint8_t check_display_count = 0; /* check the display state */
volatile uint8_t display_increment = 0;   /* increment the display state */

/*******************_OLED_DISPLAY_SETTINGS_******************************/
#define SCREEN_WIDTH 128                                                   /* OLED display width, in pixels */
#define SCREEN_HEIGHT 64                                                   /* OLED display height, in pixels*/
#define OLED_RESET -1                                                      /* Reset pin # (or -1 if sharing Arduino reset pin) */
#define SCREEN_ADDRESS 0x3C                                                /*< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET); /* Initialize the OLED display using Wire library */

/*******************_OLED_DISPLAY_STATE_MACHINE_DATA_******************************/
const int NUM_ITEMS = 4; // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)

const char *menu_items[NUM_ITEMS] =
    { // array with item names
        "*** Welcome ***",
        "*** Conectivitate ***",
        "*** Termostat ***",
        "*** Calitate aer ***"};

enum oledState
{
  ECRAN_0,
  ECRAN_1,
  ECRAN_2,
  ECRAN_3
};

oledState currentOledState = ECRAN_0;

const char *stateToString_oledState(oledState oled_state)
{
  switch (oled_state)
  {
  case ECRAN_0:
    return menu_items[0];
  case ECRAN_1:
    return menu_items[1];
  case ECRAN_2:
    return menu_items[2];
  case ECRAN_3:
    return menu_items[3];
  default:
    return "UNKNOWN";
  }
}

/***************************************************************************************************************************/
/*|_____________________________________________________LCD_DISPLAY_______________________________________________________|*/
/***************************************************************************************************************************/
/* LCD_DISPLAY_PINS: --> D21: I2C SDA - Serial data
                     --> D22: I2C SCL - Serial clock
                     (ESP 32 Pinout diagram)
*/

/*************_LCD_DISPLAY_SETTINGS_********************/
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2); /* Set the LCD address to 0x3F for a 16 chars and 2 line display */

/*##########################################################################################################################*/
/*|_____________________________________________CLIMATE_CONTROL:___________________________________________________________|*/
/*##########################################################################################################################*/

//_____________STATES:
enum State /* enum of the states of the system    */
{
  OFF,     /* OFF     :   Fan & Heating OFF       */
  COOLING, /* COOLING :   Fan ON & Heating OFF    */
  HEATING  /* HEATING :   Heating ON & Fan OFF    */
};

//_______INITIAL_STATE:
State currentState = OFF; /* Initial state at system start       */

//_____STATE_TO_STRING:
const char *stateToString(State state) /* Function that returns the state as a string */
{
  switch (state) /* Switch case for the state */
  {
  case OFF: /* OFF     : 0  */
    return "OFF";
  case COOLING: /* COOLING : 1  */
    return "COOLING";
  case HEATING: /* HEATING : 2  */
    return "HEATING";
  default:
    return "UNKNOWN"; /* UNKNOWN : -1 */
  }
}

//_RELAY_CONTROL_FLAGS:
bool f_fanControlTask = false;  /*  Flag for Fan  Control - flag false --> true: flag ; decision -->  flag false  */
bool f_heatControlTask = false; /*  Flag for Heat Control - flag false --> true: flag ; decision -->  flag false  */

/*##########################################################################################################################*/
/*|__________________________________________________VARS,PINS:____________________________________________________________|*/
/*##########################################################################################################################*/

/****************************************************************************************************************************/
/*____________________________________________________VARIABILES____________________________________________________________*/
/****************************************************************************************************************************/
/* const declaration: for saftey reasons + compiling is done quicker in this way (read only vars, cannot be written ?) */
/* #define declarations for the data types that accept this, task priority and handlers not                            */

/****************************************************************************************************************************/
/*_____________________________________________________PINS_________________________________________________________________*/
/****************************************************************************************************************************/

//________________INPUT_PINS: (will be added in the future)
/* FOR I2C BUS: --> D21: I2C SDA - Serial data
                --> D22: I2C SCL - Serial clock
                (ESP 32 Pinout diagram)     */
// #define pin_LDRSensorPin 20 /* Pin for LDR */

//_______________OUTPUT_PINS:
#define pin_heatRelayPin 13 /* Heat relay  pin  (toggle heat ON/OFF)                               */
#define pin_fanRelayPin 12  /* Fan relay   pin  (toggle fan ON/OFF)                                */

#define pin_heatRelayPinGND 33 /* Heat relay  GND pin  (toggle heat ON/OFF)                          */
#define pin_fanRelayPinGND 32  /* Fan relay   GND pin  (toggle fan ON/OFF)                           */

#define pin_ledBuiltInPin 2 /* LedBuiltIn  pin  (signal if connection to MQTT Server is realized)  */
#define pin_ledRedPin 5     /* Pin for led(in role of lights ON/OFF)                               */

#define BUTTON_UP_PIN 4 /* Menu button pin */
#define UP_LED_PIN 14   /* Signal LED pin */

/*##########################################################################################################################*/
/*|____________________________________________________TASKS:______________________________________________________________|*/
/*##########################################################################################################################*/

/****************************************************************************************************************************/
/*____________________________________________TASK_DATA_LOG_FUNCTION________________________________________________________*/
/****************************************************************************************************************************/

void dataLog(char *taskName, int taskPrio) /* Function that displays the data of the task */
{
  Serial.print("[#] ");
  Serial.print(taskName); /* Task name */
  Serial.print(": ");

  Serial.print(" prio: ");
  Serial.print(taskPrio); /* Task priority */

  Serial.print(" Core: ");
  Serial.print(xPortGetCoreID()); /* Core where the task in running */

  Serial.println(" | ");
  // Serial.println(" -> Running OK ");
}

/****************************************************************************************************************************/
/*_______________________________________________TASK_PRIORITIES____________________________________________________________*/
/****************************************************************************************************************************/

// __TASK_PRIORITY__
// --> Highest_prio : lower number value, lowest prio: bigger number value

//_Core_0_Tasks_prio:

//__Sensor_Tasks:
const static uint8_t p_shtBmeReadTask = 10; /*  >> shtReadTask priority: MEDIUM          */

//_Display_Tasks:
const static uint8_t p_displayStateTask = 20;   /*  >> displayStateTask priority: MEDIUM     */
const static uint8_t p_sensorsDisplayTask = 20; /*  >> sensorsDisplayTask priority: MEDIUM   */

//_Control_Tasks:
const static uint8_t p_fanControlTask = 25;  /*  >> fanControlTask priority: MEDIUM       */
const static uint8_t p_heatControlTask = 25; /*  >> heatControlTask priority: MEDIUM      */

//_Core_1_Tasks_prio:

//____Diag_Tasks:
const static uint8_t p_scanI2CTask = 1; /*  >> scanI2CTask priority: HIGH            */

//_____Com_Tasks:
const static uint8_t p_setupWiFiTask = 2;               /*  >> setupWiFiTask priority: HIGH          */
const static uint8_t p_MQTT_CommunciationTask = 5;      /*  >> MQTT_CommunciationTask priority: HIGH */
const static uint8_t p_MQTT_publishDataTask = 6;        /*  >> MQTT_CommunciationTask priority: HIGH */
const static uint8_t p_MQTT_publishSystemStateTask = 6; /*  >> MQTT_CommunciationTask priority: HIGH */
const static uint8_t p_uartComTask = 10;                /*  >> uartComTask priority: MEDIUM */
const static uint8_t p_oledLcdTask = 1;                 /*  >> oledDisplayTask priority: HIGH */

//__TIME_CONSTARINS__(ms):

#define tc_moduleSetup_delay 50            /* Delay set between each part of the setup modules                                                  */
#define tc_objCreate_delay 1000            /* Delay set between initialization of each object part of a module in setup                         */
#define tc_taskFunc_delay 25               /* Short delay before and after task routine executes                                                */
#define tc_quickTask_delay 1000            /* Delay for short tasks (ex: display tasks etc.)                                                    */
#define tc_measureTask_delay 2500          /* Delay for sensor tasks                                                                            */
#define tc_controlTask_delay 1000          /* Delay for control tasks (ex: relay control tasks etc.)                                            */
#define tc_wifiSetupScan_delay 500         /* Delay for WiFi setup - set the time between pings for trying to connect to WiFi networks          */
#define tc_mqttReconnect_delay 5000        /* Delay for MQTT Server reconnect - Wait 5 seconds before retrying                                  */
#define tc_ESPReset_delay 1000             /* Delay for ESP restart                                                                             */
#define tc_MQTT_publishDataTask_delay 2000 /* Delay for publishig data to MQTT Server topics                                                    */
#define tc_MQTT_publishSysState_delay 3000 /* Delay for publishig system state to MQTT Server topics                                            */

/****************************************************************************************************************************/
/*______________________________________________STACK_SIZE_(in_bytes)_______________________________________________________*/
/****************************************************************************************************************************/
#define STACK_SIZE_BYTES 4095 /* Stack size in bytes */

/****************************************************************************************************************************/
/*______________________________________________ TASK_HANDLERS______________________________________________________________*/
/****************************************************************************************************************************/

//_Core_0_Task_handlers:

//__Sensor_Tasks:
TaskHandle_t h_shtBmeReadTask; /* >> shtReadTask handler            */

//_Display_Tasks:
TaskHandle_t h_displayStateTask;   /* >> displayStateTask handler       */
TaskHandle_t h_sensorsDisplayTask; /* >> sensorsDisplayTask handler     */

//_Control_Tasks:
TaskHandle_t h_fanControlTask;  /* >> fanControlTask handler         */
TaskHandle_t h_heatControlTask; /* >> heatControlTask handler        */

//_Core_1_Task_handlers:

//____Diag_Tasks:
TaskHandle_t h_scanI2CTask; /* >> scanI2CTask handler            */

//_____Com_Tasks:
TaskHandle_t h_setupWiFiTask;               /* >> setupWiFiTask handler          */
TaskHandle_t h_MQTT_CommunciationTask;      /* >> MQTT_CommunciationTask handler */
TaskHandle_t h_MQTT_publishDataTask;        /* >> MQTT_CommunciationTask handler */
TaskHandle_t h_MQTT_publishSystemStateTask; /* >> MQTT_CommunciationTask handler */
TaskHandle_t h_uartComTask;                 /* >> uartComTask handler*/
TaskHandle_t h_oledLcdTask;                 /* >> oledDisplayTask handler*/

/****************************************************************************************************************************/
/*__________________________________________________MUTEX_HANDLERS__________________________________________________________*/
/****************************************************************************************************************************/
bool taskCanRun;
SemaphoreHandle_t mutex_setup; /* Mutex handler */

/****************************************************************************************************************************/

#endif /* DEFINES */
