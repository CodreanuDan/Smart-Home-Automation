/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: UART_COM_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de comunicarea prin UART

/*   ---> Modulul contine functiile task-ului: Task :        -> functia principala a task-ului:                     -> T_uartComTask(void* parameter)
                                                             -> funtia care se ocupa de conectarea la retele WiFi:  -> uartCom()
                                                             */

/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef UART_COM_TASK_DEFINITIONS
#define UART_COM_TASK_DEFINITIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/

/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
class UART_Com
{
private:
    static void parseData(String uart_rx_data_to_parse); /* Function responsable for parsing the received data */
public:
    UART_Com();
    static void uartCom(); /* Function responsable for UART communication */
};

extern UART_Com uart_com;

/****************************************************************************************************************************/
#endif /* UART_COM_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef UART_COM_TASK_FUNCTIONS
#define UART_COM_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_uartComTask(void *parameter) /* Main function of the Task */
{
    for (;;)
    {
        char *taskName = "T_uartComTask"; /* Task name for dataLog function */

        /*________TASK_SUB-ROUTINES___________*/

        dataLog(taskName, p_uartComTask); /* Task info */

        uart_com.uartCom(); /* Function responsable for UART communication */

        /*____________________________________*/

        vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay)); /* Delay the task */
    }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*______________________________________________RECV_UART_DATA_FUNCTION_____________________________________________________*/
/****************************************************************************************************************************/
void UART_Com::uartCom() /* Function responsable UART communication*/
{

    Serial2.print("ESP32_TX");

    if (Serial2.available())
    {                                                             /* Check if data is available to read */
        String uart_rx_data = Serial2.readString();               /* Read the incoming data */
        Serial.println("   ");                                    /* Print received data to Serial monitor */
        Serial.println("[UART_2_COM] Received: " + uart_rx_data); /* Print received data to Serial monitor */

        uart_com.parseData(uart_rx_data); /* Parse the received data */
    }
}

/****************************************************************************************************************************/
/*_____________________________________________________PARSE_DATA___________________________________________________________*/
/****************************************************************************************************************************/
void UART_Com::parseData(String uart_rx_data_to_parse) /* Function responsable for parsing the received data */
{
    const char* receivedData  = uart_rx_data_to_parse.c_str();

    int parsed = sscanf(receivedData, "TX_%f_%f_%f_%f", &iaqData.v_eCO2, &iaqData.v_EtOH, &iaqData.v_TVOC, &iaqData.v_IAQ);

    // Check if the parsing was successful
    if (parsed == 4)
    {
        // The data was parsed successfully
        Serial.println("[UART_2_COM] Data parsed successfully!");
    }
    else
    {
        // There was an error parsing the data
        Serial.println("[UART_2_COM] Error parsing the data!");
    }
}

/****************************************************************************************************************************/
#endif /* UART_COM_TASK_FUNCTIONS */
