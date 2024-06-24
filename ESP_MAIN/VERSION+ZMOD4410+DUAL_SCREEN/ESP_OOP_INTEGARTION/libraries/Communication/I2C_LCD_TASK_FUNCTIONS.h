/****************************************************************************************************************************/
/*___________________________________________________DESCRIERE_MODUL________________________________________________________*/
/****************************************************************************************************************************/

/*   ---> MODULE NAME: I2C_LCD_TASK_FUNCTIONS.h */

/*   ---> Modulul contine functiile task-ului care se ocupa de afisarea pe un display LCD prin I2C si ECRAN OLED 0.96" prin I2C

/*   ---> Modulul contine functiile task-ului: Task :       -> functia principala a task-ului:    -> T_oledLcdTask(void* parameter)
                                                            -> funtia care se ocupa de afisarea pe display:  -> oled_lcd_display_update(String oled_message, uint8_t cursor_1, uint8_t cursor_2, uint8_t text_size, String lcd_first_row_msg, String lcd_second_row_msg)
                                                            -> funtia care se ocupa de schimbarea informatiilor afisate:  -> change_display_information()
                                                            -> funtia care se ocupa de incrementarea indexului de afisare:  -> get_button_increment()
                                                            -> funtia care se ocupa de verificarea apasarii butonului:  -> manage_button_pressing()
                                                             */
/*##########################################################################################################################*/
/*_____________________________________________________DEFINITIONS__________________________________________________________*/
/*##########################################################################################################################*/
#ifndef I2C_LCD_TASK_DEFINITIONS
#define I2C_LCD_TASK_DEFINITIONS

/****************************************************************************************************************************/
/*______________________________________________________INCLUDES____________________________________________________________*/
/****************************************************************************************************************************/
#include <LiquidCrystal_I2C.h>/* Library for the I2C LCD */
#include <Adafruit_GFX.h>     /* Library for the OLED display */
#include <Adafruit_SSD1306.h> /* Library for the OLED display */
/*__LIBRARIES__*/

/*__MODULES__*/
#include "DEFINES.h" /*  Includes all the variables used on the program*/

/****************************************************************************************************************************/
/*__________________________________________________SUB_ROUTINES____________________________________________________________*/
/****************************************************************************************************************************/
class Display_data_LCD_OLED /* Class for the display data on the LCD and OLED display */
{
private:
    static void oled_lcd_display_update(String oled_message, uint8_t cursor_1,
                                        uint8_t cursor_2, uint8_t text_size,
                                        String lcd_first_row_msg, String lcd_second_row_msg);       /* Function responsable for updating the display */
    static int manage_button_pressing();                                                            /* Function responsable for checking the button press */
    static void updateDisplayIfChanged(String newMessage, String newFirstRow, String newSecondRow); /* Function responsable for updating the display if changed */
public:
    Display_data_LCD_OLED();                                                            /* Constructor */
    static int get_button_increment();                                                  /* Function responsable getting the index of button pressing */
    static void change_display_information(uint8_t display_index, struct tm &timeinfo); /* Function responsable for changing the display information */
};

extern Display_data_LCD_OLED displayDataLCD_OLED; /* Object of the class */

/****************************************************************************************************************************/
#endif /* I2C_LCD_TASK_DEFINITIONS */

/*##########################################################################################################################*/
/*____________________________________________________TASK_FUNCTIONS________________________________________________________*/
/*##########################################################################################################################*/
#ifndef I2C_LCD_TASK_FUNCTIONS
#define I2C_LCD_TASK_FUNCTIONS

/****************************************************************************************************************************/
/*____________________________________________________TASK_FUNCTION_________________________________________________________*/
/****************************************************************************************************************************/
void T_oledLcdTask(void *parameter) /* Main function of the Task */
{
    for (;;)
    {
        char *taskName = "T_oledLcdTask"; /* Task name for dataLog function */

        /*________TASK_SUB-ROUTINES___________*/

        struct tm timeinfo;           /* Time structure */
        if (!getLocalTime(&timeinfo)) /* If the time is not available */
        {
            Serial.println("[NTP_SERVER] Failed to obtain time !"); /* Print the error message */
            return;                                                 /* Return */
        }
        volatile uint8_t oled_screen_number;                                          /* Variable for the OLED screen number */
        oled_screen_number = displayDataLCD_OLED.get_button_increment();              /* Get the index of button pressing */
        displayDataLCD_OLED.change_display_information(oled_screen_number, timeinfo); /* Change the display information */

        /*____________________________________*/

        vTaskDelay(pdMS_TO_TICKS(tc_quickTask_delay / 20)); /* Delay the task 50 ms*/
    }
}

//__TASK_SUB-ROUTINES__

/****************************************************************************************************************************/
/*_____________________________________________DISPLAY_UPDATE_FUNCTION______________________________________________________*/
/****************************************************************************************************************************/
void Display_data_LCD_OLED::oled_lcd_display_update(String oled_message, uint8_t cursor_1, uint8_t cursor_2,
                                                    uint8_t text_size, String lcd_first_row_msg, String lcd_second_row_msg) /* Function responsable for updating the display */
{

    /*******************************************************/
    /*______________________OLED_DISPLAY___________________*/
    /*******************************************************/
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(cursor_1, cursor_2);
    display.println(oled_message);
    display.display();

    /*******************************************************/
    /*______________________LCD_DISPLAY____________________*/
    /*******************************************************/
    lcd.setCursor(0, 0);
    lcd.println(lcd_first_row_msg);

    lcd.setCursor(0, 1);
    lcd.println(lcd_second_row_msg);
}

/****************************************************************************************************************************/
/*___________________________________________CHANGE_DISPLAY_INFO_FUNCTION___________________________________________________*/
/****************************************************************************************************************************/
void Display_data_LCD_OLED::change_display_information(uint8_t display_index, struct tm &timeinfo) /* Function responsable for changing the display information */
{

    String oled_message;
    String wifiNetoworkIP;
    String tempString;
    String thresholdString;
    String humidityString;
    String iaqString;
    String lcd_first_row_msg;
    String lcd_second_row_msg;
    String co2String;
    String tvocString;

    switch (display_index)
    {
    case 0:
        currentOledState = ECRAN_0;
        /****************************************************************************************************************************/
        char timeStr[16];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S %d/%m   ", &timeinfo);

        lcd_first_row_msg = "Smart Home Dan.C";
        lcd_second_row_msg = timeStr;

        if (currentOledState == ECRAN_0)
        {
            String title = String(stateToString_oledState(currentOledState));
            int spaces = (6 - title.length()) / 2;
            oled_message = String(' ', spaces) + "  " + title + 
                         "\n  Smart home" + 
                         "\n  by Dan Codreanu" +
                         "\n  " + timeStr;

            displayDataLCD_OLED.updateDisplayIfChanged(oled_message, lcd_first_row_msg, lcd_second_row_msg);
            displayDataLCD_OLED.oled_lcd_display_update(oled_message, 0, 0, 1, lcd_first_row_msg, lcd_second_row_msg);
        }
        /****************************************************************************************************************************/

        break;

    case 1:
        currentOledState = ECRAN_1;
        /****************************************************************************************************************************/

        wifiNetoworkIP = WiFi.localIP().toString();
        lcd_first_row_msg = "Conectivitate   ";
        lcd_second_row_msg = "IP: " + wifiNetoworkIP;

        if (currentOledState == ECRAN_1)
        {
            String title = String(stateToString_oledState(currentOledState));
            int spaces = (6 - title.length()) / 2;
            oled_message = String(' ', spaces/2) + "" + title +
                           "\n *Wifi: " + ssid +
                           "\n *IP:" + wifiNetoworkIP +
                           "\n *MQTT: " + mqttStatus +
                           "\n *BLE:";
            displayDataLCD_OLED.updateDisplayIfChanged(oled_message, lcd_first_row_msg, lcd_second_row_msg);
            displayDataLCD_OLED.oled_lcd_display_update(oled_message, 0, 0, 1, lcd_first_row_msg, lcd_second_row_msg);
        }
        /****************************************************************************************************************************/

        break;

    case 2:
        currentOledState = ECRAN_2;
        /****************************************************************************************************************************/

        tempString = String(avgVal.v_final_temperatureValue, 1);
        humidityString = String(int(avgVal.v_final_humidityValue));
        thresholdString = String(receivedData.v_temperatureThreshold, 1);
        lcd_first_row_msg = "Temp:" + tempString + " Rh:" + humidityString + " ";
        if (currentState == 0)
        {
            lcd_second_row_msg = "Set:" + thresholdString + " " + stateToString(currentState) + "     ";
        }
        else
        {
            lcd_second_row_msg = "Set:" + thresholdString + " " + stateToString(currentState);
        }
        if (currentOledState == ECRAN_2)
        {
            String title = String(stateToString_oledState(currentOledState));
            int spaces = (6 - title.length()) / 2;
            oled_message = String(' ', spaces) + "  " + title + 
                         "\n *Temp.: " + tempString + "C" + 
                         "\n *Termosat: " + thresholdString + 
                         "\n *Umidiate: " + humidityString + " %" + 
                         "\n *Stare Clim.:" + stateToString(currentState);
            displayDataLCD_OLED.updateDisplayIfChanged(oled_message, lcd_first_row_msg, lcd_second_row_msg);
            displayDataLCD_OLED.oled_lcd_display_update(oled_message, 0, 0, 1, lcd_first_row_msg, lcd_second_row_msg);
        }
        /****************************************************************************************************************************/

        break;

    case 3:
        currentOledState = ECRAN_3;
        /****************************************************************************************************************************/

        iaqString = String(iaqData.v_IAQ, 1); // One decimal place
        co2String = String(int(iaqData.v_eCO2));
        tvocString = String(int(iaqData.v_TVOC));
        lcd_first_row_msg = "Calitate aer:" + iaqString;
        lcd_second_row_msg = "CO2:" + co2String + " ppm     ";

        if (currentOledState == ECRAN_3)
        {
            String title = String(stateToString_oledState(currentOledState)).substring(0);
            int spaces = (6 - title.length()) / 2;
            oled_message = String(' ', spaces) + " " + title +
                           "\n *IAQ: " + iaqString +
                           "\n *CO2: " + co2String + " ppm" +
                           "\n *TVOC: " + tvocString + " ppb" +
                           "\n *ZMOD stat: " + String(iaqData.s_sensorStat);

            displayDataLCD_OLED.updateDisplayIfChanged(oled_message, lcd_first_row_msg, lcd_second_row_msg);
            displayDataLCD_OLED.oled_lcd_display_update(oled_message, 0, 0, 1, lcd_first_row_msg, lcd_second_row_msg);
        }
        /****************************************************************************************************************************/

        break;
    }
}

/****************************************************************************************************************************/
/*_______________________________________________CHANGE_INFO_IF_CHANGE______________________________________________________*/
/****************************************************************************************************************************/
void Display_data_LCD_OLED::updateDisplayIfChanged(String newMessage, String newFirstRow, String newSecondRow)
{
    static String oldMessage = "";
    static String oldFirstRow = "";
    static String oldSecondRow = "";

    if (newMessage != oldMessage || newFirstRow != oldFirstRow || newSecondRow != oldSecondRow)
    {
        lcd.clear();
        displayDataLCD_OLED.oled_lcd_display_update(newMessage, 0, 0, 1, newFirstRow, newSecondRow);
        oldMessage = newMessage;
        oldFirstRow = newFirstRow;
        oldSecondRow = newSecondRow;
    }
}

/****************************************************************************************************************************/
/*___________________________________________GET_BUTTON_PRESS_INDEX_FUNCTION________________________________________________*/
/****************************************************************************************************************************/
int Display_data_LCD_OLED::get_button_increment() /* Function responsable for getting the index of button pressing */
{
    check_display_count = displayDataLCD_OLED.manage_button_pressing(); /* Check if the button is pressed */
    if (check_display_count == 1)                                       /* If the button is pressed */
    {

        display_increment += 1; /* Increment the display index */
        lcd.clear();            /* Clear the display */

        if (display_increment == 4) /* If the display index is 4 */
        {
            display_increment = 0; /* Reset the display index */
            lcd.clear();           /* Clear the display */
        }
    }

    return display_increment;
}

/****************************************************************************************************************************/
/*_____________________________________________CHECK_BUTTON_STATE_FUNCTION__________________________________________________*/
/****************************************************************************************************************************/
int Display_data_LCD_OLED::manage_button_pressing() /* Function responsable for checking the button press */
{

    if ((digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 0)) /* If the button is pressed */
    {
        button_up_clicked = 1; /* Set the button state to pressed */
        // Serial.println("Up button pressed");
        digitalWrite(UP_LED_PIN, HIGH); /* Turn on the LED */
    }
    else if (digitalRead(BUTTON_UP_PIN) == HIGH) /* If the button is released */
    {
        button_up_clicked = 0; /* Set the button state to released */
        // Serial.println("Up button released");
        digitalWrite(UP_LED_PIN, LOW); /* Turn off the LED */
    }

    return button_up_clicked; /* Return the button state */
}

/****************************************************************************************************************************/
#endif /* I2C_LCD_TASK_FUNCTIONS */