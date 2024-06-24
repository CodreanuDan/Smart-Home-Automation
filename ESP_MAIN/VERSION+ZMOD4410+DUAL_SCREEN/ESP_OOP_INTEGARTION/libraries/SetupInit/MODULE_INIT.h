/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: MODULE_INIT.h */

/*   ---> Modulul contine functii de initializare a modulelor:  --> pinInit() - Initializeaza pinii GPIO
                                                                --> mqttInit() - Initializeaza comunicatia MQTT
                                                                --> wireInit() - Initializeaza comunicatia I2C
                                                                --> mutexInit() - Initializeaza mutexul pentru sincronizarea task-urilor
                                                                --> uart2Init() - Initializeaza UART 2
                                                                --> lcdInit() - Initializeaza display-ul LCD
                                                                --> oledInit() - Initializeaza display-ul OLED

                                                              */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef MODULE_INIT_DEFINITIONS
#define MODULE_INIT_DEFINITIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*_______________________________________________CLASS_FUNCTIONS____________________________________________________________*/
/****************************************************************************************************************************/
class SetupInit/*  Class that contains the functions for the initialization of the modules */
{
public:
  SetupInit();
  static void wireInit();  /*  Function that initializes the I2C Communication */
  static void pinInit();   /*  Function that initializes the GPIO Pins */
  static void uart2Init(); /* Function that initializes UART 2 */
  static void mqttInit();  /*  Function that initializes the MQTT Communication */
  static void mutexInit(); /* Function that creates and initialse the mutex for task sync */
  static void lcdInit();   /* Function that initializes the LCD Display */
  static void oledInit();  /* Function that initializes the OLED Display */
};

extern SetupInit setupInit; /*  Object of the class SetupInit */

/****************************************************************************************************************************/
#endif /* MODULE_INIT_DEFINITIONS */

/*##########################################################################################################################*/
/*___________________________________________________CLASS_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef MODULE_INIT_FUNCTIONS
#define MODULE_INIT_FUNCTIONS
/***********************************************************************************************************************************************************/
/*_____________________________________________________________________WIRE_BEGIN_(I2C):___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::wireInit() /*  Function that initializes the I2C Communication */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|________________________________________________________WIRE_BEGIN_____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  /**********_Default_I2C_Begin_*************/
  Wire.begin(); /* Begin communication with I2C bus */
  Serial.println("[Default_I2C_BUS]: Initialized!"); /* Print message to Serial Monitor */
  delay(tc_moduleSetup_delay);

  /************_OLED_I2C_Begin_**************/
  Wire1.begin(SDA_PIN_OLED, SCL_PIN_OLED); /* Begin communication with I2C bus for OLED Display */
  Serial.println("[OLED_I2C_BUS]: Initialized!"); /* Print message to Serial Monitor */
  delay(tc_moduleSetup_delay);

}
/***********************************************************************************************************************************************************/
/*__________________________________________________________________SERIAL_BEGIN_(UART):___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::uart2Init() /* Function that initializes UART 2 */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________UART_2_INIT_____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN); /*  Initialize Serial2 with RX and TX pins */
  delay(tc_moduleSetup_delay);
}

/***********************************************************************************************************************************************************/
/*_________________________________________________________________________PINMODE_INIT:___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::pinInit() /* Function that initializes the GPIO Pins */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________PINMODE_INIT____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__________OUTPUT_PINS:

  pinMode(pin_heatRelayPin, OUTPUT); /* >> pinMode: heatRelayPin: as OUTPUT. (Controls the toggle of the HEAT ON/OFF)*/
  digitalWrite(pin_heatRelayPin, LOW);
  Serial.print("  >> pinMode: heatRelayPin:");
  Serial.print(pin_heatRelayPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_fanRelayPin, OUTPUT); /* >> pinMode: fanRelayPin: as OUTPUT. (Controls the toggle of the FAN ON/OFF) */
  digitalWrite(pin_fanRelayPin, LOW);
  Serial.print("  >> pinMode: fanRelayPin:");
  Serial.print(pin_fanRelayPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_heatRelayPinGND, OUTPUT); /* >> pinMode: heatRelayPin: as OUTPUT. (Controls the toggle of the HEAT ON/OFF)*/
  digitalWrite(pin_heatRelayPinGND, LOW);
  Serial.print("  >> pinMode: heatRelayPinGND:");
  Serial.print(pin_heatRelayPinGND);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_fanRelayPinGND, OUTPUT); /* >> pinMode: fanRelayPin: as OUTPUT. (Controls the toggle of the FAN ON/OFF) */
  digitalWrite(pin_fanRelayPinGND, LOW);
  Serial.print("  >> pinMode: fanRelayPinGND:");
  Serial.print(pin_fanRelayPinGND);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_ledBuiltInPin, OUTPUT); /* >> pinMode: ledBuiltInPin: as OUTPUT. (Signals if connection to MQTT Server is succsesful*/
  digitalWrite(pin_ledBuiltInPin, LOW);
  Serial.print("  >> pinMode: ledBuiltInPin:");
  Serial.print(pin_ledBuiltInPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_ledRedPin, OUTPUT); /* >> pinMode: ledRedPin: as OUTPUT. (In role of light control)*/
  digitalWrite(pin_ledRedPin, LOW);
  Serial.print("  >> pinMode: ledRedPin:");
  Serial.print(pin_ledRedPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(UP_LED_PIN, OUTPUT); /* >> pinMode: UP_LED_PIN : as OUTPUT. (to sigal button press)*/
  digitalWrite(UP_LED_PIN, LOW);
  Serial.print("  >> pinMode: UP_LED_PIN:");
  Serial.print(UP_LED_PIN);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  //___________INPUT_PINS:

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  ; /* >> pinMode: BUTTON_UP_PIN: as BUTTON_UP_PIN. (for changing display info )*/
  Serial.print("  >> pinMode: BUTTON_UP_PIN:");
  Serial.print(BUTTON_UP_PIN);
  Serial.println(" as INPUT_PULLUP.");
  delay(tc_objCreate_delay);

  //__PINMODE_INIT_END__
  delay(tc_moduleSetup_delay);
}

/***********************************************************************************************************************************************************/
/*_________________________________________________________________________LCD_INIT:___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::lcdInit() /* Function that initializes the LCD Display */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________LCD_INIT_______________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__LCD_INIT__
  Serial.println("<*> LCD: --> Initialize LCD Display<*>");

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("** Smart Home **");

  lcd.setCursor(0, 1);
  lcd.print("by Dan C.-Setup");

  //__LCD_INIT_END__
  delay(tc_moduleSetup_delay);
}

/***********************************************************************************************************************************************************/
/*_________________________________________________________________________OLED_INIT:___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::oledInit() /* Function that initializes the OLED Display */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________OLED_INIT______________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__OLED_INIT__
  Serial.println("<*> OLED: --> Initialize OLED Display<*>");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; /* Don't proceed, loop forever */
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("DISPLAY ON!");
  display.display();

  //__OLED_INIT_END__
  delay(tc_moduleSetup_delay);
}
/***********************************************************************************************************************************************************/
/*____________________________________________________________________________MQTT_INIT:___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::mqttInit() /*  Function that initializes the MQTT Communication */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|______________________________________________________MQTT_COM_INIT____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__MQTT_SERVER_CONNECTION__
  Serial.println("<*> MQTT: --> Establish connection to MQTT server, PORT 1883 <*>");
  client.setServer(mqtt_server, 1883); /*  Set the MQTT Server IP and the PORT */
  delay(tc_objCreate_delay);

  //__MQTT_CALLBACK_SET__
  Serial.println("<*> MQTT: --> Set callback function<*>");
  client.setCallback(mqttCOM.callback); /*  Set callback function: Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */
  delay(tc_objCreate_delay);
}

/***********************************************************************************************************************************************************/
/*___________________________________________________________________________MUTEX_INIT:___________________________________________________________________*/
/***********************************************************************************************************************************************************/
void SetupInit::mutexInit() /* Function that creates and initialse the mutex for task sync */
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|______________________________________________________MUTEX_INIT_______________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__MUTEX_INIT__
  Serial.println("<*> Mutex: --> Creating Mutex<*>");
  mutex_setup = xSemaphoreCreateMutex(); /* Create the mutex */
  delay(tc_objCreate_delay);

  //__MUTEX_INIT_END__
  delay(tc_moduleSetup_delay);
}

/****************************************************************************************************************************/
#endif /* MODULE_INIT_FUNCTIONS */
