/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: MODULE_INIT.h */

/*   ---> Modulul o functii de initializare a modulelor: --> pinInit()
                                                         --> mqttInit()
                                                         --> mq2Init()
                                                         --> wireInit()
                                                         -->
                                                              */

#ifndef MODULE_INIT
#define MODULE_INIT

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/


/*__MODULES__*/
#include "DEFINES.h"                                                                                                  /*  Includes all the variables used on the program*/

/***********************************************************************************************************************************************************/
/*_____________________________________________________________________WIRE_BEGIN_(I2C):___________________________________________________________________*/   
/***********************************************************************************************************************************************************/
void wireInit()
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|________________________________________________________WIRE_BEGIN_____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");
  
  Wire.begin();                                                                                                       /* Begin communication with I2C bus */
  delay(tc_moduleSetup_delay);

  //________________________________________________INFO__
  Serial.print("<INFO> setup() running on core ");
  Serial.println(xPortGetCoreID());                                                                                   /* See on wich core setup() runs */
  delay(tc_moduleSetup_delay);
}

/***********************************************************************************************************************************************************/
/*_________________________________________________________________________PINMODE_INIT:___________________________________________________________________*/   
/***********************************************************************************************************************************************************/
void pinInit()
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|_______________________________________________________PINMODE_INIT____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__________OUTPUT_PINS:

  pinMode(pin_heatRelayPin, OUTPUT);                                                                                  /* >> pinMode: heatRelayPin: as OUTPUT. (Controls the toggle of the HEAT ON/OFF)*/
  Serial.print("  >> pinMode: heatRelayPin:");
  Serial.print(pin_heatRelayPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_fanRelayPin, OUTPUT);                                                                                   /* >> pinMode: fanRelayPin: as OUTPUT. (Controls the toggle of the FAN ON/OFF) */
  Serial.print("  >> pinMode: fanRelayPin:");
  Serial.print(pin_fanRelayPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_ledBuiltInPin, OUTPUT);                                                                                 /* >> pinMode: ledBuiltInPin: as OUTPUT. (Signals if connection to MQTT Server is succsesful*/
  digitalWrite(pin_ledBuiltInPin, LOW);
  Serial.print("  >> pinMode: ledBuiltInPin:");
  Serial.print(pin_ledBuiltInPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  pinMode(pin_ledRedPin, OUTPUT);                                                                                     /* >> pinMode: ledRedPin: as OUTPUT. (In role of light control)*/
  digitalWrite(pin_ledRedPin, LOW);
  Serial.print("  >> pinMode: ledRedPin:");
  Serial.print(pin_ledRedPin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);

  //___________INPUT_PINS:

  pinMode(pin_mq2Pin, INPUT);                                                                                         /* >> pinMode: mq2Pin: as INPUT. (Reads data from Analog pin A0 from MQ2 gas sensor*/
  Serial.print("  >> pinMode: pin_mq2Pin:");
  Serial.print(pin_mq2Pin);
  Serial.println(" as OUTPUT.");
  delay(tc_objCreate_delay);


  //__PINMODE_INIT_END__
  delay(tc_moduleSetup_delay);

}

/***********************************************************************************************************************************************************/
/*____________________________________________________________________________MQ2_CAL:_____________________________________________________________________*/   
/***********************************************************************************************************************************************************/
void mq2Init()
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|____________________________________________________MQ2_SENSOR_INIT____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  Serial.println("<MQ2> [Calibration]> Calibrating...");                
  // Ro = MQCalibration(pin_mq2Pin);                                                                                      /* Calibrating the sensor. Please   make sure the sensor is in clean air when   you perform the calibration */                  
  Serial.println("<MQ2> [Calibration]> Calibration is done..."); 

  Serial.print("<MQ2> [Calibration]> Ro = ");
  // Serial.print(Ro);
  Serial.print(" kohm");
  Serial.println();

}

/***********************************************************************************************************************************************************/
/*____________________________________________________________________________MQTT_INIT:___________________________________________________________________*/   
/***********************************************************************************************************************************************************/
void mqttInit()
{
  Serial.println("###########################################################################################################################");
  Serial.println("#|______________________________________________________MQTT_COM_INIT____________________________________________________|#");
  Serial.println("###########################################################################################################################");
  Serial.println("  ");

  //__MQTT_SERVER_CONNECTION__
  Serial.println("<*> MQTT: --> Establish connection to MQTT server, PORT 1883 <*>");
  client.setServer(mqtt_server,1883);                                                                                 /*  Set the MQTT Server IP and the PORT */
  delay(tc_objCreate_delay);

  //__MQTT_CALLBACK_SET__
  Serial.println("<*> MQTT: --> Set callback function<*>");
  client.setCallback(callback);                                                                                       /*  Set callback function: Subroutine for MQTT Communication Task responsable for showing the received messages and making decisions accrodingly */
  delay(tc_objCreate_delay);

}

/****************************************************************************************************************************/

#endif                                                           /* MODULE_INIT */


