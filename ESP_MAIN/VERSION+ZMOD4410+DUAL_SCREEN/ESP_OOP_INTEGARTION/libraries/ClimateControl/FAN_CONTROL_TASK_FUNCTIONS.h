/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: _TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de controlul ventilatorului: OFF <--> COOLING

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                 -> T_fanControlTask(void* parameter)
                                                             -> functia de control a ventilatorului:            -> fanControl()
                                                             -> functia de activare/dezactivare:                -> toggleFan(bool state)
                                                             -> functia de activare/dezactivare a incalzirii:   -> toggleHeat0(bool state)
                                                             -> functia de semanlizare a starii ventilatorului: -> fanOnSignal()
                                                             -> functia de ajustare a histerezei:              -> hystersisAdjustment()
                                                             */
/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef FAN_CONTROL_TASK_DEFINITIONS
#define FAN_CONTROL_TASK_DEFINITIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/
#include <math.h>

/****************************************************************************************************************************/
/*______________________________________________CLASS_FOR_TASK_SUB_ROUTINES_________________________________________________*/
/****************************************************************************************************************************/
class FanControlClass
{
private:
  static void toggleFan(bool state);   /* Toggles the FAN --> ON/OFF */
  static void toggleHeat0(bool state); /* Toggles the HEAT --> ON/OFF when no threshold is recived from MQTT Server (0)*/
  static int sgn(int val);             /* Returns the sign of the value */
public:
  FanControlClass();
  static void fanControl();           /* Controls the Fan. Calls dhtReadTask() & toggleFan(bool state). Can change the currentState between OFF <--> COOLING */
  static void fanOnSignal();          /* Signals the currentState for Fan Control */
  static float hystersisAdjustment(); /* Adjusts the hysteresis of the system */
};

extern FanControlClass fan_controller;

/****************************************************************************************************************************/
#endif /* FAN_CONTROL_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef FAN_CONTROL_TASK_FUNCTIONS
#define FAN_CONTROL_TASK_FUNCTIONS
/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_fanControlTask(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "fanControlTask"; /* Task name for dataLog function */

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_fanControlTask); /* Task Info */

    fan_controller.fanOnSignal(); /* Displays the state of the fan */

    fan_controller.fanControl(); /* Controls the fan */

    /*____________________________________*/

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
  }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*___________________________________________________FAN_CONTROL_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void FanControlClass::fanControl() /* Controls the Fan. Can change the currentState between OFF <--> COOLING */
{
  if (!f_fanControlTask) /* Checks if the fan control flag is false, if false becomes true and there can be made changes on the fan control */
  {
    f_fanControlTask = true; /* The flag becomes true */

    float v_hyst_val = fan_controller.hystersisAdjustment(); /* Calls the hystersisAdjustment function to adjust the hysteresis of the system */

    float v_temperature_lowerLimit = receivedData.v_temperatureThreshold - v_hyst_val; /* Calculates the lower limit of the temperature */
    float v_temperature_upperLimit = receivedData.v_temperatureThreshold + v_hyst_val; /* Calculates the upper limit of the temperature */

    float tolerance = 0.55; // Tolerance value

    if (abs(avgVal.v_final_temperatureValue - receivedData.v_temperatureThreshold) < tolerance)
    {
      currentState = OFF;
      fan_controller.toggleFan(false);
    }
    else
    {
      switch (currentState) /* Switch the current system state between OFF and COOLING by comparing the temperature with the threshold */
      {
      case OFF:
        if (avgVal.v_final_temperatureValue > v_temperature_upperLimit) /* If the average temperature is greater than the threshold + hysteresis value */
        {
          currentState = COOLING;
          fan_controller.toggleFan(true); /* Calls the toggleFan function to switch the fan ON */
        }
        break;
      case COOLING:
        if (avgVal.v_final_temperatureValue <= v_temperature_lowerLimit) /* If the average temperature is less than the threshold - hysteresis value */
        {
          currentState = OFF;
          fan_controller.toggleFan(false); /* Calls the toggleFan function to switch the fan OFF */
        }
        break;
      }
    }

    if (receivedData.v_temperatureThreshold == 0) /* Switches OFF the Fan and Heating when no threshold is received */
    {
      currentState = OFF;
      fan_controller.toggleFan(false);
      fan_controller.toggleHeat0(false);
    }

    f_fanControlTask = false; /* After the whole operation the flag becomes false until the task is activated again */
  }
}
/****************************************************************************************************************************/
/*___________________________________________________TOGGLE_FAN__FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void FanControlClass::toggleFan(bool state) /* Toggles the FAN --> ON/OFF */
{
  digitalWrite(pin_fanRelayPin, state ? LOW : HIGH); /* Toggles the fan by checking the currentState */
  digitalWrite(pin_fanRelayPinGND, state ? LOW : HIGH); /* Toggles the fan by checking the currentState */
  Serial.println(state ? "Fan is ON" : "Fan is OFF");
}

/****************************************************************************************************************************/
/*___________________________________________________TOGGLE_HEAT_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void FanControlClass::toggleHeat0(bool state) /* Toggles the HEAT --> ON/OFF when no threshold is recived from MQTT Server (0)*/
{
  digitalWrite(pin_heatRelayPin, state ? LOW : HIGH); /* Toggles the heat by checking the currentState */
  digitalWrite(pin_heatRelayPinGND, state ? LOW : HIGH); /* Toggles the heat by checking the currentState */
  Serial.println(state ? "Heat is ON" : "Heat is OFF");
}

/****************************************************************************************************************************/
/*______________________________________________________FAN_INFO_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void FanControlClass::fanOnSignal() /* Signals the currentState for Fan Control */
{
  if (currentState == COOLING)
  {
    Serial.println(" -> FAN: ON");
  }
  else if (currentState == OFF)
  {
    Serial.println(" -> FAN: OFF");
  }
  else
  {
    Serial.println(" -> FAN: OFF");
  }
}

/****************************************************************************************************************************/
/*________________________________________________HYSTERSESIS_ADJUSTMENT_FUNCTION___________________________________________*/
/****************************************************************************************************************************/
float FanControlClass::hystersisAdjustment() /* Adjusts the hysteresis of the system */
{
  float v_tempDiffernece = avgVal.v_final_temperatureValue - receivedData.v_temperatureThreshold; /* Calculates the difference between the inside and outside temperature */
  float v_absDifference = abs(v_tempDiffernece);                                                  /* Calculates the absolute value of the difference */
  int signDifference = fan_controller.sgn(v_tempDiffernece);

  if (v_absDifference < 0.5)
  {
    return 0;
  }
  else if (v_absDifference >= 20)
  {
    return signDifference * 1.0;
  }
  else if (v_absDifference >= 15)
  {
    return signDifference * 0.85;
  }
  else if (v_absDifference >= 10)
  {
    return signDifference * 0.75;
  }
  else if (v_absDifference >= 5)
  {
    return signDifference * 0.65;
  }
  else
  {
    return signDifference * 0.5;
  }
}

/****************************************************************************************************************************/
/*_______________________________________________________SIGN_FUNCTION______________________________________________________*/
/****************************************************************************************************************************/
int FanControlClass::sgn(int val) /* Returns the sign of the value */
{
  return (val > 0) - (val < 0);
}

/****************************************************************************************************************************/
#endif /* FAN_CONTROL_TASK_FUNCTIONS */