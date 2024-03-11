/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: HEAT_CONTROL_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de controlul incalzirii: OFF <--> HEATING

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                 -> T_heatControlTask(void* parameter)
                                                             -> functia de control a incalzirii:                -> heatControl()
                                                             -> functia de activare/dezactivare:                -> toggleHeat(bool state)
                                                             -> functia de semanlizare a starii incalzirii:     -> heatOnSignal()  
                                                             */

#ifndef HEAT_CONTROL_TASK_FUNCTIONS
#define HEAT_CONTROL_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h"                                                                  /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void heatControl();                                                                   /* Controls the Heating. Calls dhtReadTask() & toggleHeat(bool state). Can change the currentState between OFF <--> HEATING */
void toggleHeat(bool state);                                                          /* Toggles the HEAT --> ON/OFF */
void heatOnSignal();                                                                  /* Signals the currentState for Heat Control */
void toggleFan0(bool state);                                                          /* Toggles the FAN --> ON/OFF when no threshold is recived from MQTT Server (0)*/ 

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_heatControlTask(void* parameter)                                              /* Main function of the Task */
{
  for(;;)
  {
    char* taskName = "heatControlTask";                                              /* Task name for dataLog function */

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/ 

    dataLog(taskName, p_heatControlTask);                                            /* Task Info */

    heatOnSignal();                                                                  /* Displays the state of the heat */      

    heatControl();                                                                   /* Controls the heat */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                   /* Delay the task */
  }

}

//__TASK_SUB-ROUTINES__


/****************************************************************************************************************************/
/*__________________________________________________HEAT_CONTROL_FUNCTION __________________________________________________*/
/****************************************************************************************************************************/
void heatControl()                                                                   /* Controls the Heating. Calls dhtReadTask() & toggleHeat(bool state). Can change the currentState between OFF <--> HEATING */
{
  if (!f_heatControlTask)                                                            /* Checks if the heat control flag is false, if false becomes true and there can be made chnges on the heat control */
    {
      f_heatControlTask = true;                                                       

      switch (currentState)                                                          /* Switch the current system state between OFF and HEAT by comparing the temperature with the threshold */   
      {
        case OFF:
          if (v_temperatureValue < v_temperatureThreshold) 
          {
            currentState = HEATING;
            toggleHeat(true);                                                        /* Calls the toggleHeat function to switch the heat ON */
          }
          else if(v_temperatureThreshold == 0)                                       /* Switches OFF the Fan and Heating when no threshold is recived */
          {
            currentState = OFF;
            toggleHeat(false);
            toggleFan0(false);
          }
          break;
        case HEATING:
          if(v_temperatureThreshold == 0)                                            /* Switches OFF the Fan and Heating when no threshold is recived */
          {
            currentState = OFF;
            if (currentState == OFF)
            {
              toggleHeat(false);
              toggleFan0(false);
            }
          }
          else if (v_temperatureValue >= v_temperatureThreshold) 
          {
            currentState = OFF;
            toggleHeat(false);                                                       /* Calls the toggleHeat function to switch the heat OFF */
          }
          break;
      }
      f_heatControlTask = false;                                                     /* After the whole operation the flag becomes false until the task is activated again (am observat ca asa se trimite comanda doar odata fara sa fie continua trimiterea de comenzi?!) */
    }

}

/****************************************************************************************************************************/
/*___________________________________________________TOGGLE_HEAT_FUNCTION __________________________________________________*/
/****************************************************************************************************************************/
void toggleHeat(bool state)                                                          /* Toggles the HEAT --> ON/OFF */
{
  digitalWrite(pin_heatRelayPin, state ? HIGH : LOW);                                /* Toggles the heat by checking the currentState */ 
  Serial.println(state ? "Heat is ON" : "Heat is OFF");
}


/****************************************************************************************************************************/
/*___________________________________________________TOGGLE_FAN__FUNCTION __________________________________________________*/
/****************************************************************************************************************************/
void toggleFan0(bool state)                                                           /* Toggles the FAN --> ON/OFF when no threshold is recived from MQTT Server (0)*/ 
{
  digitalWrite(pin_fanRelayPin, state ? HIGH : LOW);                                  /* Toggles the fan by checking the currentState */                 
  Serial.println(state ? "Fan is ON" : "Fan is OFF");
}


/****************************************************************************************************************************/
/*_____________________________________________________HEAT_INFO_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void heatOnSignal()                                                                  /* Signals the currentState for Heat Control */
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

#endif                                                                               /* HEAT_CONTROL_TASK_FUNCTIONS */


