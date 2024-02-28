/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SHT_DISPLAY_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de afisarea datelor de la senzorul SHT 21 (temp + umid. aer)

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:               -> T_shtDisplayTask(void* parameter)
                                                             -> functia de afisare a datelor de la senzor:    -> shtDataDisplay()
                                                             */

#ifndef SHT_DISPLAY_TASK_FUNCTIONS
#define SHT_DISPLAY_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h"                                                                /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void shtDataDisplay();                                                              /* Function that displays the data from the SHT 21 Sensor */


/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_shtDisplayTask(void* parameter)                                              /* Main function of the Task */
{
  for(;;)
  {
    char* taskName = "shtDisplayTask";

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/ 

    dataLog(taskName, p_shtDisplayTask);                                            /* Task info */

    shtDataDisplay();                                                               /* SHT 21 Sensor data display */

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                  /* Delay the task */
  }

}

//__TASK_SUB-ROUTINES__


/****************************************************************************************************************************/
/*_________________________________________________SHT_DATA_DSIPLAY_FUNCTION________________________________________________*/
/****************************************************************************************************************************/
void shtDataDisplay()                                                               /* Function that displays the data from the SHT 21 Sensor */
{
    Serial.print(" -> Temp: ");
    Serial.print(v_temperatureValue);                                               /* Temperature read from the sensor via shtReadTask (float value)*/
    Serial.print(" C* | ");

    Serial.print("Hum: ");
    Serial.print(v_humidityValue);                                                  /* Air humidity read from the sensor via shtReadTask (float value)*/
    Serial.println(" % "); 

}

/****************************************************************************************************************************/

#endif                                                                              /* SHT_DISPLAY_TASK_FUNCTIONS */


