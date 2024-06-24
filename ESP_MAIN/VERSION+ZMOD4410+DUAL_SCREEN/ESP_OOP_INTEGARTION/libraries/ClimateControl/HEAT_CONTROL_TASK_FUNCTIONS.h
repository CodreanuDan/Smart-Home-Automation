/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: HEAT_CONTROL_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de controlul incalzirii: OFF <--> HEATING

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                 -> T_heatControlTask(void* parameter)
                                                             -> functia de control a incalzirii:                -> heatControl()
                                                             -> functia de activare/dezactivare:                -> toggleHeat(bool state)
                                                             -> functia de activare/dezactivare a ventilatorului: -> toggleFan0(bool state)
                                                             -> functia de semanlizare a starii incalzirii:     -> heatOnSignal()
                                                             -> functia de ajustare a histerezei:               -> hystersisAdjustment()
                                                             */
/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef HEAT_CONTROL_TASK_DEFINITIONS
#define HEAT_CONTROL_TASK_DEFINITIONS

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
class HeatControlClass
{
private:
  static void toggleFan0(bool state); /* Toggles the FAN --> ON/OFF when no threshold is recived from MQTT Server (0)*/
  static void toggleHeat(bool state); /* Toggles the HEAT --> ON/OFF */
  static int sgn(int val);            /* Returns the sign of the value */
public:
  HeatControlClass();                 /* Constructor */
  static void heatControl();          /* Controls the Heating. Calls dhtReadTask() & toggleHeat(bool state). Can change the currentState between OFF <--> HEATING */
  static void heatOnSignal();         /* Signals the currentState for Heat Control */
  static float hystersisAdjustment(); /* Adjusts the hysteresis of the heating system */
};

extern HeatControlClass heat_controller; /* Object of the class */
/****************************************************************************************************************************/
#endif /* HEAT_CONTROL_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef HEAT_CONTROL_TASK_FUNCTIONS
#define HEAT_CONTROL_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_heatControlTask(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "heatControlTask"; /* Task name for dataLog function */

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_heatControlTask); /* Task Info */

    heat_controller.heatOnSignal(); /* Displays the state of the heat */

    heat_controller.heatControl(); /* Controls the heat */

    /*____________________________________*/

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
  }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*__________________________________________________HEAT_CONTROL_FUNCTION __________________________________________________*/
/****************************************************************************************************************************/
void HeatControlClass::heatControl() /* Controls the Heating. Can change the currentState between OFF <--> HEATING */
{
  if (!f_heatControlTask) /* Checks if the heat control flag is false, if false becomes true and there can be made changes on the heat control */
  {
    f_heatControlTask = true; /* The flag becomes true */

    float v_hyst_val = heat_controller.hystersisAdjustment(); /* Calls the hystersisAdjustment function to adjust the hysteresis of the heating system */

    float v_temperature_lowerLimit = receivedData.v_temperatureThreshold - v_hyst_val; /* Calculates the lower limit of the temperature */
    float v_temperature_upperLimit = receivedData.v_temperatureThreshold + v_hyst_val; /* Calculates the upper limit of the temperature */
    
    float tolerance = 0.55; // Tolerance value

    if (abs(avgVal.v_final_temperatureValue - receivedData.v_temperatureThreshold) < tolerance) {
        currentState = OFF;
        heat_controller.toggleHeat(false);
    } else {
        switch (currentState) /* Switch the current system state between OFF and HEATING by comparing the temperature with the threshold */
        {
        case OFF:
          if (avgVal.v_final_temperatureValue < (v_temperature_lowerLimit - tolerance)) /* If the average temperature is less than the threshold - hysteresis value - tolerance */
          {
            currentState = HEATING;
            heat_controller.toggleHeat(true); /* Calls the toggleHeat function to switch the heat ON */
          }
          break;
        case HEATING:
          if (avgVal.v_final_temperatureValue >= v_temperature_upperLimit) /* If the average temperature is greater than the threshold + hysteresis value */
          {
            currentState = OFF;
            heat_controller.toggleHeat(false); /* Calls the toggleHeat function to switch the heat OFF */
          }
          break;
        }
    }

    if (receivedData.v_temperatureThreshold == 0) /* Switches OFF the Fan and Heating when no threshold is received */
    {
      currentState = OFF;
      heat_controller.toggleHeat(false);
      heat_controller.toggleFan0(false);
    }

    f_heatControlTask = false; /* After the whole operation the flag becomes false until the task is activated again */
  }
}

/****************************************************************************************************************************/
/*___________________________________________________TOGGLE_HEAT_FUNCTION __________________________________________________*/
/****************************************************************************************************************************/
void HeatControlClass::toggleHeat(bool state) /* Toggles the HEAT --> ON/OFF */
{
  digitalWrite(pin_heatRelayPin, state ? LOW : HIGH); /* Toggles the heat by checking the currentState */
  digitalWrite(pin_heatRelayPinGND, state ? LOW : HIGH); /* Toggles the heat by checking the currentState */
  Serial.println(state ? "Heat is ON" : "Heat is OFF");
}

/****************************************************************************************************************************/
/*__________________________________________________TOGGLE_FAN__FUNCTION ___________________________________________________*/
/****************************************************************************************************************************/
void HeatControlClass::toggleFan0(bool state) /* Toggles the FAN --> ON/OFF when no threshold is recived from MQTT Server (0)*/
{
  digitalWrite(pin_fanRelayPin, state ? LOW : HIGH); /* Toggles the fan by checking the currentState */
  digitalWrite(pin_fanRelayPinGND, state ? LOW : HIGH); /* Toggles the fan by checking the currentState */
  Serial.println(state ? "Fan is ON" : "Fan is OFF");
}

/****************************************************************************************************************************/
/*___________________________________________________HEAT_INFO_FUNCTION_____________________________________________________*/
/****************************************************************************************************************************/
void HeatControlClass::heatOnSignal() /* Signals the currentState for Heat Control */
{

  if (currentState == HEATING)
  {
    Serial.println("-> HEAT: ON");
  }
  else if (currentState == OFF)
  {
    Serial.println("-> HEAT: OFF");
  }
  else
  {
    Serial.println("-> HEAT: OFF");
  }
}

/****************************************************************************************************************************/
/*________________________________________________HYSTERSESIS_ADJUSTMENT_FUNCTION___________________________________________*/
/****************************************************************************************************************************/
float HeatControlClass::hystersisAdjustment() /* Adjusts the hysteresis of the system */
{
  float v_tempDiffernece = avgVal.v_final_temperatureValue - receivedData.v_temperatureThreshold; /* Calculates the difference between the inside and outside temperature */
  float v_absDifference = abs(v_tempDiffernece);                                                  /* Calculates the absolute value of the difference */
  int signDifference = heat_controller.sgn(v_tempDiffernece);

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
/*______________________________________________________SGN_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
int HeatControlClass::sgn(int val) /* Returns the sign of the value */
{
  return (val > 0) - (val < 0);
}

/****************************************************************************************************************************/
#endif /* HEAT_CONTROL_TASK_FUNCTIONS */