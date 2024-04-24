/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SHT21_BME680_Readings.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de citirea datelor de la senzorul SHT21 (tempratura si umidiatea aerului)

/*   ---> Modulul contine functiile task-ului: shtReadTask : -> functia principala a task-ului:       -> T_shtReadTask(void* parameter)
                                                             -> functia de citire de la senzor:       -> shtRead() 
                                                             -> functia de verificare a citirii:      -> checkReadsSht()
                                                            */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SHT21_BME680_TASK_DEFINITIONS                                                            
#define SHT21_BME680_TASK_DEFINITIONS                                                                   

  /****************************************************************************************************************************/
  /*______________________________________________________INCLUDES____________________________________________________________*/
  /****************************************************************************************************************************/

  /*__LIBRARIES__*/
  #include <SHT21.h>                                                                           /* SHT 21 Sensor library */
  #include <Adafruit_Sensor.h>                                                                 /* Adafruit Sensor library */                    
  #include "Adafruit_BME680.h"                                                                 /* BME680 Sensor library */ 

  /*__MODULES__*/ 
  #include "DEFINES.h"                                                                         /*  Includes all the variables used on the program*/


  /****************************************************************************************************************************/
  /*__________________________________________________SUB_ROUTINES____________________________________________________________*/
  /****************************************************************************************************************************/
  class Sensors_SHT21_BME680_Group                                                             /* Class that contains all the functions for the SHT21 and BME680 sensors */
  {
    private:                                                                                   /* Private members of the class */
      static void shtRead();                                                                   /* Function that reads the data from SHT21 Sensor via I2C */                              
      static void bmeRead();                                                                   /* Function that reads the data from BME680 Sensor via I2C */

    public:                                                                                    /* Public members of the class */
      Sensors_SHT21_BME680_Group();                                                            /* Constructor of the class */            
      static void read_and_check_SHT21();                                                      /* Function that reads and verifies the data from SHT21 Sensor via I2C */
      static void read_and_check_BME680();                                                     /* Function that reads and verifies the data from BME680 Sensor via I2C */
      static void calc_average_Temp_Hum();                                                     /* Function that calculates the average temperature and humidity values */                    
      static void showData();                                                                  /* Function that shows the data on the Serial Monitor */                       
      static void check_sensor_reads(char* taskName);                                          /* Function that checks if any reads failed and exit early (to try again) */
  };
  extern Sensors_SHT21_BME680_Group sensorsGroup;                                              /*  Object of the class Sensors_SHT21_BME680_Group */ 
  /****************************************************************************************************************************/
  /*________________________________________________________SHT_21____________________________________________________________*/
  /****************************************************************************************************************************/
  /* SHT 21 PINS: --> D21: I2C SDA - Serial data
                  --> D22: I2C SCL - Serial clock
                  (ESP 32 Pinout diagram - I2C bus pins           */               
  static SHT21 sht;                                                                            /* SHT21 sensor object */
  /****************************************************************************************************************************/
  /*_______________________________________________________BME_680____________________________________________________________*/
  /****************************************************************************************************************************/
  /* BME680 PINS: --> D21: I2C SDA - Serial data
                  --> D22: I2C SCL - Serial clock
                  (ESP 32 Pinout diagram - I2C bus pins           */               
  static Adafruit_BME680 bme;                                                                  /* BME680 sensor object */

/****************************************************************************************************************************/
#endif                                                                                         /* SHT21_BME680_TASK_DEFINITIONS */



/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SHT21_BME680_TASK_FUNCTIONS
#define SHT21_BME680_TASK_FUNCTIONS

  /****************************************************************************************************************************/
  /*____________________________________________________TASK_FUNCTION_________________________________________________________*/
  /****************************************************************************************************************************/
  void T_shtBmeReadTask(void* parameter)                                                       /* Main function of the shtReadTask */
  {
  // asta e de verifcat sa vad daca merge cu mutex
    for(;;)
    {
      if(xSemaphoreTake(mutex_setup, portMAX_DELAY) == pdTRUE)                                    /* Take the mutex */
      {
        
      char* taskName = "shtBmeReadTask";                                                      /* Task name for dataLog function */

      delay(tc_taskFunc_delay);

      /*________TASK_SUB-ROUTINES___________*/ 
      /**************************************/  

      sensorsGroup.read_and_check_SHT21();                                                    /* Read and check the data from SHT21 sensor */
      sensorsGroup.read_and_check_BME680();                                                   /* Read and check the data from BME680 sensor */
      sensorsGroup.calc_average_Temp_Hum();                                                   /* Calculate the average temperature and humidity values */                
      sensorsGroup.check_sensor_reads(taskName);                                              /* Check if any reads failed and exit early (to try again) */               
      sensorsGroup.showData();                                                                /* Show the data on the Serial Monitor */                   

      /**************************************
      /*____________________________________*/

      delay(tc_taskFunc_delay);                                                                /* Delay the task */
      xSemaphoreGive(mutex_setup);                                                             /* Give the mutex */

      vTaskDelay(pdMS_TO_TICKS(tc_measureTask_delay));                                         /* Delay the task */
      
      } 
      else 
      {
        Serial.println("[#] shtBmeReadTask() : Failed to take the mutex!");
      }
    }
    
  }

  //__TASK_SUB-ROUTINES__                                                                      /* Sub-routines of the task */


  /****************************************************************************************************************************/
  /*_______________________________________________SHT_21_READINGS_FUNCTION___________________________________________________*/
  /****************************************************************************************************************************/
  void Sensors_SHT21_BME680_Group::shtRead()                                                   /*  Function that reads the data from SHT21 Sensor via I2C */
  {
    shtData.v_sht_temperatureValue = sht.getTemperature();                                     /*  Read temperature from sensor */
    shtData.v_sht_humidityValue    = sht.getHumidity();                                        /*  Read humidity from sensor */

  }

  /****************************************************************************************************************************/
  /*_______________________________________SHT_21_CHECK_READINGS_FUNCTION_____________________________________________________*/
  /****************************************************************************************************************************/
  void Sensors_SHT21_BME680_Group::read_and_check_SHT21()                                      /* Function that checks if any reads failed and exit early (to try again) */
  {
    sensorsGroup.shtRead();                                                                    /* Read the data from SHT21 sensor */
    shtReadFlag = false;

    if (isnan(shtData.v_sht_temperatureValue) || isnan(shtData.v_sht_humidityValue))           /* Check if any reads failed and exit early (to try again) */
    {
      Serial.println("[#] shtBmeReadTask() : Failed to read from SHT21 sensor!");
      return;                                                                                  /* Try again ! */
    } 
    else 
    {
      shtReadFlag = true;                                                                      /* Set the flag to true */                
    } 

  }

  /****************************************************************************************************************************/
  /*__________________________________________BME_680_READINGS_FUNCTION_______________________________________________________*/
  /****************************************************************************************************************************/
  void Sensors_SHT21_BME680_Group::bmeRead()                                                   /*  Function that reads the data from BME680 Sensor via I2C */
  {
    bme.begin();                                                                               /* Start the BME680 sensor */             
    bme.beginReading();                                                                        /* Start the BME680 sensor reading */           

    bme.setTemperatureOversampling(BME680_OS_8X);                                              /* Set the oversampling for temperature */
    bme.setHumidityOversampling(BME680_OS_2X);                                                 /* Set the oversampling for humidity */
    bme.setPressureOversampling(BME680_OS_4X);                                                 /* Set the oversampling for pressure */        
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);                                                /* Set the IIR filter size */   
    bme.setGasHeater(320, 150);                                                                /* Set the gas heater 320*C for 150 ms    */          

    bmeData.v_bme_temperatureValue = bme.temperature;                                          /* Read temperature from sensor */
    bmeData.v_bme_humidityValue    = bme.humidity;                                             /* Read humidity from sensor */            

    bmeData.v_bme_pressureValue = round(bme.pressure / 100.0 * 10) / 10;  /* Read pressure from sensor */
    bmeData.v_bme_gasResValue = round(bme.gas_resistance / 1000.0 * 10) / 10;  /* Read gas resistance from sensor */
    bmeData.v_bme_ReadAltValue = round(bme.readAltitude(SEALEVELPRESSURE_HPA) * 10) / 10;  /* Read altitude from sensor */

  }

  /****************************************************************************************************************************/
  /*______________________________________BME_680_CHECK_READINGS_FUNCTION_____________________________________________________*/
  /****************************************************************************************************************************/
  void Sensors_SHT21_BME680_Group::read_and_check_BME680()                                     /* Function that checks if any reads failed and exit early (to try again) */
  {
    sensorsGroup.bmeRead();                                                                    /* Read the data from BME680 sensor */
    bmeReadFlag = false;
    if (!bme.performReading())                                                                 /* Try to read sensor */
    {
        Serial.println("[#] shtBmeReadTask() : Failed to read from BME680 sensor!");
        return;                                                                                /* Try again ! */
    } 
    else 
    {
        bmeReadFlag = true;                                                                    /* Set the flag to true */              
    }

  }

  /****************************************************************************************************************************/
  /*________________________________________CHECK_BOTH_SENSOR_READINGS_FUNCTION_______________________________________________*/
  /****************************************************************************************************************************/
  void Sensors_SHT21_BME680_Group::check_sensor_reads(char* taskName)                          /* Function that checks if any reads failed and exit early (to try again) */
  {
    if (shtReadFlag && bmeReadFlag)                                                            /* Check if any reads failed and exit early (to try again) */
    {
        dataLog(taskName, p_shtBmeReadTask);                                                   /* Log the data */
        Serial.println("[#] shtBmeReadTask: Read OK!"); 

    }
    else                                                                                       /* Check if any reads failed and exit early (to try again) */
    {
         Serial.print("[#] shtBmeReadTask: Read Failed!");
    }
  }

  /****************************************************************************************************************************/
  /*_____________________________________________AVERAGE_VALUE(TEMP_&_RH)_____________________________________________________*/
  /****************************************************************************************************************************/
  void Sensors_SHT21_BME680_Group::calc_average_Temp_Hum()                                     /* Function that calculates the average temperature and humidity values */ 
  {
    avgVal.v_final_temperatureValue = round((bmeData.v_bme_temperatureValue + shtData.v_sht_temperatureValue) / 2.0 * 10) / 10;  /* Calculate the average temperature value */
    avgVal.v_final_humidityValue    = round((bmeData.v_bme_humidityValue + shtData.v_sht_humidityValue) / 2.0 * 10) / 10;  /* Calculate the average humidity value */

  }

  /****************************************************************************************************************************/
  /*____________________________________________________SHOW_DATA_____________________________________________________________*/
  /****************************************************************************************************************************/  
  void Sensors_SHT21_BME680_Group::showData()                                                  /* Function that shows the data on the Serial Monitor */
  {
    Serial.print(F("Final Temperature: ")); Serial.println(String(avgVal.v_final_temperatureValue));
    Serial.print("Final Humidity: ")   ; Serial.println(String(avgVal.v_final_humidityValue));
    
    // Serial.print("BME Temperature: ")  ; Serial.println(String(bmeData.v_bme_temperatureValue));
    // Serial.print("BME Humidity: ")     ; Serial.println(String(bmeData.v_bme_humidityValue));

    // Serial.print("SHT Temperature: ")  ; Serial.println(String(shtData.v_sht_temperatureValue));
    // Serial.print("SHT Humidity: ")     ; Serial.println(String(shtData.v_sht_humidityValue));
    Serial.println();

  }



/****************************************************************************************************************************/
#endif                                                                                         /* SHT21_BME680_TASK_FUNCTIONS */


