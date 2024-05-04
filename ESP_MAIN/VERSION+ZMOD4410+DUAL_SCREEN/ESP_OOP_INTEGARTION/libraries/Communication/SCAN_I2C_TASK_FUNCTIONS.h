/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SCAN_I2C_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de scanarea de dispozitive ce comunica pe interfata I2C

/*   ---> Acest Task se executa doar odata la pornirea programului

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:     -> T_scanI2CTask(void* parameter)
                                                             -> functia de scanare I2C:             -> void scanI2C()
                                                             */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SCAN_I2C_TASK_DEFINITIONS
#define SCAN_I2C_TASK_DEFINITIONS

  /****************************************************************************************************************************/
  /*______________________________________________________INCLUDES____________________________________________________________*/
  /****************************************************************************************************************************/

  /*__LIBRARIES__*/
  #include <Wire.h>                                                                           /* I2C Interface library */

  /*__MODULES__*/
  #include "DEFINES.h"                                                                        /*  Includes all the variables used on the program*/

  /****************************************************************************************************************************/
  /*__________________________________________________SUB_ROUTINES____________________________________________________________*/
  /****************************************************************************************************************************/
  class I2C_BUS
  {
    public:
      I2C_BUS();
      static void scanI2C();                                                                             /* Scans connection to I2C Adresses, returns them */
  };

  extern I2C_BUS i2c_bus;
/****************************************************************************************************************************/
#endif                                                                                    /* SCAN_I2C_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef SCAN_I2C_TASK_FUNCTIONS
#define SCAN_I2C_TASK_FUNCTIONS

  /****************************************************************************************************************************/
  /*____________________________________________________TASK_FUNCTION_________________________________________________________*/
  /****************************************************************************************************************************/
  void T_scanI2CTask(void* parameter)                                                         /* Main function of the Task */
  {
    //for(;;)
    //{
      char* taskName = "scanI2CTask";                                                         /* Task name for dataLog function */

      /*________TASK_SUB-ROUTINES___________*/

      dataLog(taskName, p_scanI2CTask);                                                       /* Task info */

      i2c_bus.scanI2C();                                                                              /* Scan for I2C decices, suspend task after scan */

      /*____________________________________*/

      //delay(tc_taskFunc_delay);

      //vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                        /* Delay the task */
    //}

  }

  //__TASK_SUB-ROUTINES__


  /****************************************************************************************************************************/
  /*______________________________________________________SCAN_I2C_FUNCTION___________________________________________________*/
  /****************************************************************************************************************************/
  void I2C_BUS::scanI2C()                                                                              /* Scans connection to I2C Adresses, returns them */
  {
    byte error, address;                                                                      /* Byte error for address and address of I2C interface device */
    int nrDevices;                                                                            /* Variable for nr. of devices found */
    bool stopTask = false;                                                                    /* Bool var for I2C scan: init as false, after the scan of devices is done -> bool = true; task is suspended */
    
    Serial.println("--> Scanning for I2C devices:");
    
    nrDevices = 0;
    for (address = 1; address < 127; address++)                                               /* Loop trough all posibile addresses */
    {
      Wire.beginTransmission(address);                                                        /* Begin communication with the device at current address */
      error = Wire.endTransmission();                                                         /* End the communication and translate it to an error */
      
      if (error == 0) 
      {
        Serial.print("--> Found I2C device at address 0x");
        if (address < 16) 
        {
          Serial.print("0");
        }
        Serial.println(address, HEX);                                                         /* The address found */
        nrDevices++;
      } 
      else if (error == 4) 
      {
        Serial.print("--> Found unknown error at address 0x");
        if (address < 16) 
        {
          Serial.print("0");
        }
        Serial.println(address, HEX);                                                         /* The address found */
      }
    }
    
    if (nrDevices == 0) 
    {
      Serial.println("--> No I2C devices found");
    }
    
    Serial.println("--> Scan complete\n");

    stopTask = true;                                                                              
    if (stopTask == true)                                                                    /* Scan is done --> task cand be suspended */
    {
      delay(500);
      vTaskSuspend(NULL);
    }

  }

/****************************************************************************************************************************/
#endif                                                                                    /* SCAN_I2C_TASK_FUNCTIONS */


