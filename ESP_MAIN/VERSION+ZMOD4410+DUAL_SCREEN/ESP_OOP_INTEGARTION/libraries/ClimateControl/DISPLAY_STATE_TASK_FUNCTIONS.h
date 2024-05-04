/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*  ---> MODULE NAME: DISPLAY_STATE_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de afisarea starii sistemului (0: OFF; 1: COOLING; 2:HEATING) si pragul de temperatura pentru termostat

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:     -> T_displayStateTask(void* parameter)
                                                             -> functia care afiseaza datele:       -> stateThresholdDisplay()
                                                             */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef DISPLAY_STATE_TASK_DEFINITIONS
#define DISPLAY_STATE_TASK_DEFINITIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
class Info_State_Threshold_DataLog
{
public:
  Info_State_Threshold_DataLog();
  static void stateThresholdDisplay(); /* Function that displays the data of the task and temperature threshold */
};

extern Info_State_Threshold_DataLog systemInfo;

/****************************************************************************************************************************/
#endif /* DISPLAY_STATE_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef DISPLAY_STATE_TASK_FUNCTIONS
#define DISPLAY_STATE_TASK_FUNCTIONS
/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_displayStateTask(void *parameter) /* Main function of the Task */
{
  for (;;)
  {
    char *taskName = "displayStateTask"; /* Task name for dataLog function */

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_displayStateTask); /* Task info */

    systemInfo.stateThresholdDisplay(); /* System info displau (state + temp.threshold) */

    /*____________________________________*/

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
  }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*______________________________________________SHT_DATA_DISPLAY_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void Info_State_Threshold_DataLog::stateThresholdDisplay() /* Function that displays the data of the task and temperature threshold */
{
  Serial.print(" <!> CURRENT STATE:");
  Serial.print(currentState); /* Current state of the system as a number */
  Serial.print(" : ");
  Serial.print(stateToString(currentState)); /* Current state of the system as a string*/
  Serial.print(" | ");
  Serial.print(" Temperature threshold:");
  Serial.print(receivedData.v_temperatureThreshold); /* Temperature threshold*/
  Serial.print(" C* ");
  Serial.print(" | ");
  Serial.print(" Outside temperature:");
  Serial.print(receivedData.v_outsideTemp); /* Outside temperature*/
  Serial.println(" C* ");
}

/****************************************************************************************************************************/
#endif /* DISPLAY_STATE_TASK_FUNCTIONS */
