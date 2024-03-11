/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SHT_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de citirea datelor de la senzorul SHT21 (tempratura si umidiatea aerului)

/*   ---> Modulul contine functiile task-ului: shtReadTask : -> functia principala a task-ului:       -> T_shtReadTask(void* parameter)
                                                             -> functia de citire de la senzor:       -> shtRead() 
                                                             -> functia de verificare a citirii:      -> checkReadsSht()
                                                            */

#ifndef SHT_TASK_FUNCTIONS
#define SHT_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/
#include <SHT21.h>                                                                         /* SHT 21 Sensor library */

/*__MODULES__*/ 
#include "DEFINES.h"                                                                       /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void shtRead();                                                                            /* Function that reads the data from SHT21 Sensor via I2C */
void checkReadsSht(char* taskName);                                                        /* Function that checkf if any reads failed and exit early (to try again) */


/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_shtReadTask(void* parameter)                                                       /* Main function of the shtReadTask */
{
  for(;;)
  {
    char* taskName = "shtReadTask";                                                       /* Task name for dataLog function */

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/                                              

    shtRead();                                                                            /* Sensor readings */

    checkReadsSht(taskName);                                                              /* Check sensor readings + dataLog (Task info)*/

    /*____________________________________*/

    delay(tc_taskFunc_delay);

    vTaskDelay(pdMS_TO_TICKS(tc_measureTask_delay));                                      /* Delay the task */

  }

}

//__TASK_SUB-ROUTINES__


/****************************************************************************************************************************/
/*___________________________________________________SHT_21_READ_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void shtRead()                                                                            /*  Function that reads the data from SHT21 Sensor via I2C */
{

  v_temperatureValue = sht.getTemperature();                                              /*  Read temperature from sensor */
  v_humidityValue = sht.getHumidity();                                                    /*  Read humidity from sensor */

  const int tc_SHT21_SensorTaskMeasure_timeStamp = 2000;                                  /*  declare 2 seconds necesary for proper seonsor readings) */
  delay(tc_SHT21_SensorTaskMeasure_timeStamp);                                            /*  measurement delay for sensor measurement  */

}

/****************************************************************************************************************************/
/*___________________________________________________CHECK_READS_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void checkReadsSht(char* taskName)                                                        /* Function that checks if any reads failed and exit early (to try again) */
{

  if (isnan(v_humidityValue) || isnan(v_temperatureValue))                                /* Check if any reads failed and exit early (to try again) */
    {
      Serial.println("[#] dhtReadTask() : Failed to read from SHT21 sensor!");
      return;                                                                             /* Try again ! */
    } 
    else 
    {
      dataLog(taskName, p_shtReadTask);                                                   /* Task info */
      Serial.println(" -> Running OK ");      
    }

}

/****************************************************************************************************************************/

#endif                                                                                    /* SHT_TASK_FUNCTIONS */


