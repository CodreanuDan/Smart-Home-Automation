/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: _TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de controlul ventilatorului: OFF <--> COOLING

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                 -> T_fanControlTask(void* parameter)
                                                             -> functia de control a ventilatorului:            -> fanControl()
                                                             -> functia de activare/dezactivare:                -> toggleFan(bool state)
                                                             -> functia de semanlizare a starii ventilatorului: -> fanOnSignal()
                                                             */

#ifndef FAN_CONTROL_TASK_FUNCTIONS
#define FAN_CONTROL_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h"                                                                   /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void fanControl();                                                                     /* Controls the Fan. Calls dhtReadTask() & toggleFan(bool state). Can change the currentState between OFF <--> COOLING */
void toggleFan(bool state);                                                            /* Toggles the FAN --> ON/OFF */
void fanOnSignal();                                                                    /* Signals the currentState for Fan Control */


/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_fanControlTask(void* parameter)                                                 /* Main function of the Task */
{
  for(;;)
  {
    char* taskName = "fanControlTask";

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/ 

    dataLog(taskName, p_fanControlTask);                                               /* Task Info */

    fanOnSignal();                                                                     /* Displays the state of the fan */                                                   

    fanControl();                                                                      /* Controls the fan */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                     /* Delay the task */
  }

}

//__TASK_SUB-ROUTINES__


/****************************************************************************************************************************/
/*___________________________________________________FAN_CONTROL_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void fanControl()                                                                      /* Controls the Fan. Calls dhtReadTask() & toggleFan(bool state). Can change the currentState between OFF <--> COOLING */
{
  if (!f_fanControlTask)                                                               /* Checks if the fan control flag is false, if false becomes true and there can be made chnges on the fan control */
    {
      f_fanControlTask = true;

      switch (currentState)                                                            /* Switch the current system state between OFF and COOLING by comparing the temperature with the threshold */ 
      {
        case OFF:
          if (v_temperatureValue > v_temperatureThreshold) 
          {
            currentState = COOLING;
            toggleFan(true);                                                           /* Calls the toggleFan function to switch the fan ON */
          }

          break;

        case COOLING:
          if (v_temperatureValue <= v_temperatureThreshold) 
          {
            currentState = OFF;
            toggleFan(false);                                                          /* Calls the toggleFan function to switch the fan OFF */
          }

          break;
      }
      f_fanControlTask = false;                                                        /* After the whole operation the flag becomes false until the task is activated again (am observat ca asa se trimite comanda doar odata fara sa fie continua ?!) */
    }

}

/****************************************************************************************************************************/
/*___________________________________________________TOGGLE_FAN__FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void toggleFan(bool state)                                                             /* Toggles the FAN --> ON/OFF */
{
  digitalWrite(pin_fanRelayPin, state ? HIGH : LOW);                                   /* Toggles the fan by checking the currentState */                 
  Serial.println(state ? "Fan is ON" : "Fan is OFF");
}

/****************************************************************************************************************************/
/*______________________________________________________FAN_INFO_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void fanOnSignal()                                                                     /* Signals the currentState for Fan Control */
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

#endif                                                                                 /* FAN_CONTROL_TASK_FUNCTIONS */


