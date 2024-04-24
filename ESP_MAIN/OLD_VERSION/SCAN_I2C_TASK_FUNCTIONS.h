/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: SCAN_I2C_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de scanarea de dispozitive ce comunica pe interfata I2C

/*   ---> Acest Task se executa doar odata la pornirea programului

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:     -> T_scanI2CTask(void* parameter)
                                                             -> functia de scanare I2C:             -> void scanI2C()
                                                             */

#ifndef SCAN_I2C_TASK_FUNCTIONS
#define SCAN_I2C_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/
#include <Wire.h>                                                                         /* I2C Interface library */

/*__MODULES__*/
#include "DEFINES.h"                                                                      /*  Includes all the variables used on the program*/


/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
void scanI2C();                                                                           /* Scans connection to I2C Adresses, returns them */


/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_scanI2CTask(void* parameter)                                                       /* Main function of the Task */
{
  //for(;;)
  //{
    char* taskName = "scanI2CTask";                                                       /* Task name for dataLog function */

    delay(tc_taskFunc_delay);

    /*________TASK_SUB-ROUTINES___________*/

    dataLog(taskName, p_scanI2CTask);                                                     /* Task info */

    scanI2C();                                                                            /* Scan for I2C decices, suspend task after scan */

    /*____________________________________*/

    //delay(tc_taskFunc_delay);

    //vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay));                                        /* Delay the task */
  //}

}

//__TASK_SUB-ROUTINES__


/****************************************************************************************************************************/
/*______________________________________________________SCAN_I2C_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void scanI2C()                                                                            /* Scans connection to I2C Adresses, returns them */
{
  byte error, address;                                                                    /* Byte error for address and address of I2C interface device */
    int nrDevices;                                                                        /* Variable for nr. of devices found */
    Serial.print("--> Scanning for I2C devices:");
    nrDevices = 0;

    for (address = 1; address < 127; address++)                                           /* Loop trough all posibile addresses */                                          
    {
      Wire.beginTransmission(address);                                                    /* Begin communication with the device at current address */
      error = Wire.endTransmission();                                                     /* End the communication and translate it to an error */
      if (error == 0) 
      {
        Serial.print("--> Found I2C device at address 0x");
        if (address < 16) 
        {
          Serial.print("0");
        }
        Serial.println(address, HEX);                                                     /* The address found */
        vTaskSuspend(NULL);                                                               /* Suspend task after scan */
      } 
      else if (error == 4) 
      {
        Serial.print("--> Found unknown error at address 0x");
        if (address < 16) 
        {
          Serial.print("0");
        }
        Serial.println(address, HEX);                                                     /* The address found */
        vTaskSuspend(NULL);                                                               /* Suspend task after scan */
      }
    }

}

/****************************************************************************************************************************/

#endif                                                                                    /* SCAN_I2C_TASK_FUNCTIONS */


