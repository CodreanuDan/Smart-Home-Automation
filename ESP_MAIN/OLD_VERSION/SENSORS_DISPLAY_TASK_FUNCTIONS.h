/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SENSORS_DISPLAY_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de afisarea datelor de la: --> senzorul SHT 21 (temp + umid. aer) 
                                                                                       --> senzorul MQ2 (nivelul de gaze din aer (ppm))
                                                                                       --> fotorezistor (nivelul de lumina)

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:               -> T_shtDisplayTask(void* parameter)
                                                             -> functia de afisare a datelor de la senzor:    -> shtDataDisplay()
                                                             */

#ifndef SENSORS_DISPLAY_TASK_FUNCTIONS
#define SENSORS_DISPLAY_TASK_FUNCTIONS

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
// void mq2DataDisplay();

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_sensorsDisplayTask(void* parameter)                                          /* Main function of the Task */
{
  for(;;)
  {
    char* taskName = "sensorsDisplayTask";                                          /* Task name for dataLog function */

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/ 

    dataLog(taskName, p_sensorsDisplayTask);                                        /* Task info */

    shtDataDisplay();                                                               /* SHT21 sensor data display (air temp. + RH) */

    // mq2DataDisplay();

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
    Serial.print("<SHT21>: ");

    Serial.print(" -> Temp: ");
    Serial.print(v_temperatureValue);                                               /* Temperature read from the sensor via shtReadTask (float value)*/
    Serial.print(" C* | ");

    Serial.print("Hum: ");
    Serial.print(v_humidityValue);                                                  /* Air humidity read from the sensor via shtReadTask (float value)*/
    Serial.print(" % "); 

    Serial.println(" ");

}


/****************************************************************************************************************************/
/*_________________________________________________MQ2_DATA_DSIPLAY_FUNCTION________________________________________________*/
/****************************************************************************************************************************/
// void mq2DataDisplay()
// {
//     Serial.print("<MQ2>: ");

//     Serial.print(" -> LPG: ");
//     Serial.print(v_LPGLevel);
//     Serial.print(" ppm");

//     Serial.print(" -> CO: ");
//     Serial.print(v_COLevel);
//     Serial.print(" ppm");

//     Serial.print(" -> Smoke: ");
//     Serial.print(v_smokeValue);
//     Serial.print(" ppm");

//     Serial.println(" ");

// }

/****************************************************************************************************************************/

#endif                                                                              /* SENSOR_DISPLAY_TASK_FUNCTIONS */


