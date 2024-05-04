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

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SENSORS_DISPLAY_TASK_DEFINITIONS
#define SENSORS_DISPLAY_TASK_DEFINITIONS

  /****************************************************************************************************************************/
  /*______________________________________________________INCLUDES____________________________________________________________*/
  /****************************************************************************************************************************/

  /*__LIBRARIES__*/

  /*__MODULES__*/
  #include "DEFINES.h"                                                                /*  Includes all the variables used on the program*/


  /****************************************************************************************************************************/
  /*__________________________________________________SUB_ROUTINES____________________________________________________________*/
  /****************************************************************************************************************************/
  class SensorsDisplay
  {
    public:
      SensorsDisplay();
      static void shtDataDisplay();                                                      /* Function that displays the data from the SHT 21 Sensor */
      static void bmeDataDisplay();                                                      /* Function that displays the data from the BME 680 Sensor */
  };

  extern SensorsDisplay sensorDisplay;
/****************************************************************************************************************************/
#endif                                                                              /* SENSOR_DISPLAY_TASK_DEFINITIONS */


/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SENSORS_DISPLAY_TASK_FUNCTIONS
#define SENSORS_DISPLAY_TASK_FUNCTIONS

  /****************************************************************************************************************************/
  /*____________________________________________________TASK_FUNCTION_________________________________________________________*/
  /****************************************************************************************************************************/
  void T_sensorsDisplayTask(void* parameter)                                          /* Main function of the Task */
  {
    for(;;)
    {
      char* taskName = "sensorsDisplayTask";                                          /* Task name for dataLog function */

      /*________TASK_SUB-ROUTINES___________*/ 

      dataLog(taskName, p_sensorsDisplayTask);                                        /* Task info */

      sensorDisplay.shtDataDisplay();                                                 /* SHT21 sensor data display (air temp. + RH) */
      sensorDisplay.bmeDataDisplay();                                                 /* BME sensor data display (air.Temp, RH, hPa, alt, gas.Res) */


      /*____________________________________*/

      vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                  /* Delay the task */
    }

  }

  //__TASK_SUB-ROUTINES__


  /****************************************************************************************************************************/
  /*_________________________________________________SHT_DATA_DSIPLAY_FUNCTION________________________________________________*/
  /****************************************************************************************************************************/
  void SensorsDisplay::shtDataDisplay()                                                /* Function that displays the data from the SHT 21 Sensor */
  {   
      Serial.print("<SHT21>: ");

      Serial.print(" -> Temp: ");Serial.print(shtData.v_sht_temperatureValue);Serial.println(" C* | "); /* Air humidity read from the sensor via shtReadTask (float value)*/
      Serial.print("->  Hum: ");Serial.print(shtData.v_sht_humidityValue);Serial.println(" % ");            /* Temperature read from the sensor via shtReadTask (float value)*/

      Serial.println(" ");

  }


  // /****************************************************************************************************************************/
  // /*_________________________________________________BME_DATA_DSIPLAY_FUNCTION________________________________________________*/
  // /****************************************************************************************************************************/
  void SensorsDisplay::bmeDataDisplay()                                                               /* Function that displays the data from the LDR */
  {   
      Serial.print("<BME680>: ");

      Serial.print(" ->  Temp: ");Serial.print(bmeData.v_bme_temperatureValue);Serial.println(" C* | ");
      Serial.print(" ->   Hum: ");Serial.print(bmeData.v_bme_humidityValue);Serial.println(" % | ");
      Serial.print(" -> Press: ");Serial.print(bmeData.v_bme_pressureValue);Serial.println(" hPa | ");
      Serial.print(" -> GasResp: ");Serial.print(bmeData.v_bme_gasResValue);Serial.println(" ohm | ");
      Serial.print(" -> Alt: ");Serial.print(bmeData.v_bme_ReadAltValue);Serial.println(" m | ");

      Serial.println(" ");

  }

/****************************************************************************************************************************/
#endif                                                                              /* SENSOR_DISPLAY_TASK_FUNCTIONS */


