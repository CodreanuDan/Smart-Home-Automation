//_________________________DESCRIERE_____________________________//
// v1.0 --> task in loop (dupa numeTask si taskPrio)
// v2.0 --> task in loop (dupa taskPrio)
// v3.0 --> Proper Task declarat in setup()
// v4.0 --> TaskPinned to Core
//
//
//_________________________INCLUDE______________________________//

#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <ACROBOTIC_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <string> // Add this line at the top of your sketch
#include "driver/gpio.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

//___________________________PINS_______________________________//

const static uint8_t p_dhtPin = 5;        // sensor data pin  (sensor data read)
const static uint8_t p_heatRelayPin = 16; // heat relay  pin  (toggle heat on/off)
const static uint8_t p_fanRelayPin = 17;  // fan relay   pin  (toggle fan on/off)

const static uint8_t p_task1PrioLED = 26; // task1PrioLED (for visualisation, signals if the task is running)
const static uint8_t p_task2PrioLED = 32; // task2PrioLED (for visualisation, signals if the task is running)
const static uint8_t p_task3PrioLED = 33; // task3PrioLED (for visualisation, signals if the task is running)
const static uint8_t p_task4PrioLED = 25; // task4PrioLED (for visualisation, signals if the task is running)
const static uint8_t p_task5PrioLED = 23; // task5PrioLED (for visualisation, signals if the task is running)

const static uint8_t p_HeatingLED = 14; // heatOnLED: signlas if the Heating is toggled ON (current State: HEATING)
const static uint8_t p_CoolingLED = 12; // fanOnLED:  signlas if the Cooling(Fan) is toggled OFF (current State:COOLING)
const static uint8_t p_OffLED = 27;     // offStateLED: signals if the current State is OFF

/*****************************************************************************************/
Adafruit_MPU6050 mpu; // MPU
/*****************************************************************************************/
#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     3
#define UTC_OFFSET_DST 3
/*****************************************************************************************/



/*#######################################################################################*/
/*#|_______________________________________OLED________________________________________|#*/
/*#######################################################################################*/
/********************************************************/
/*____________________DISPLAY_PINS______________________*/

#define BUTTON_UP_PIN 4                               /* Menu button pin */
#define UP_LED_PIN 13                                 /* Signal LEG pin */

/********************************************************/
/*____________________CONTROL_VARS______________________*/

volatile uint8_t button_up_clicked   = 0;             /* only perform action when button is clicked, and wait until another press */
volatile uint8_t check_display_count = 0;
volatile uint8_t display_increment   = 0;

/********************************************************/
/*______________________TASK_DATA_______________________*/

const static uint8_t p_oledDisplayTask = 1;
TaskHandle_t h_oledDisplayTask;

/********************************************************/
/*__________________DISPLAY_SETTINGS____________________*/

#define SCREEN_WIDTH 128                  // OLED display width, in pixels
#define SCREEN_HEIGHT 64                  // OLED display height, in pixels
#define OLED_RESET -1                     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C               ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

/********************************************************/
/*_________________STATE_MACHNE_DATA____________________*/
const int NUM_ITEMS = 4; // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)

const char *menu_items [NUM_ITEMS] =
{ // array with item names
  "Welcome",
  "Conectivitate",
  "Termostat",
  "Calitate aer"
};

enum oledState
{
  ECRAN_0,
  ECRAN_1,
  ECRAN_2,
  ECRAN_3
};

oledState currentOledState = ECRAN_0;

/*#######################################################################################*/

//___________________________DEFINE_____________________________//

//__DHT_SENSOR__
#define DHTTYPE DHT22       // type of dht sensor 11/22
DHT dht(p_dhtPin, DHTTYPE); // init the sensor

//____________________________STATES____________________________//

//__CLIMATE_CONTROL_STATES__
enum State // enum of the states of the system
{
  OFF,     // OFF     :   Fan & Heating OFF
  COOLING, // COOLING :   Fan ON & Heating OFF
  HEATING  // HEATING :   Heating ON & Fan OFF
};
State currentState = OFF; // Initial state at sys start

const char *stateToString(State state)
{
  switch (state)
  {
    case OFF: // OFF     : 0
      return "OFF";
    case COOLING: // COOLING : 1
      return "COOLING";
    case HEATING: // HEATING : 2
      return "HEATING";
    default:
      return "UNKNOWN";
  }
}

const char *stateToString_oledState(oledState oled_state)
{
  switch (oled_state)
  {
    case ECRAN_0:
      return menu_items[0];
    case ECRAN_1:
      return menu_items[1];
    case ECRAN_2:
      return menu_items[2];
    case ECRAN_3:
      return menu_items[3];
    default:
      return "UNKNOWN";
  }
}


void printTaskStackUsage(const char *taskName) {
  UBaseType_t stackLeft = uxTaskGetStackHighWaterMark(NULL); // Get remaining stack space
  Serial.print("Task ");
  Serial.print(taskName);
  Serial.print(" stack usage: ");
  Serial.print(2000 - stackLeft);
  Serial.print(" bytes used (");
  Serial.print(stackLeft);
  Serial.println(" bytes left)");
}

void doSomething() {
  // Implementează acțiunea dorită când butonul este apăsat
  Serial.println("Button pressed!");
  // Poți adăuga orice alt cod pentru a răspunde la evenimentul de apăsare a butonului
}
//____________________________VARS______________________________//

//__TASK_DELAY__
const static uint8_t task_delay = 255;
//__DHT_VALUES__
float v_temperatureValue; // DHT temp (C*)
float v_humidityValue;    // DHT RH (%)

//__MUTEX__
bool f_fanControlTask = false;  //  Flag for Fan Control - flag false --> true: flag ;decision -->  flag false
bool f_heatControlTask = false; //  Flag for Heat Control - flag false --> true: flag ;decision --> flag false

//__TASK_PRIORITY__
const static uint8_t p_dhtReadTask = 2;
const static uint8_t p_displayStateTask = 3;
const static uint8_t p_dhtDisplayTask = 4;
const static uint8_t p_fanControlTask = 2;
const static uint8_t p_heatControlTask = 2;
const static uint8_t p_mpu6050Task = 2;
const static uint8_t p_NTPClockTask = 4;
// const static uint8_t p_oledDisplayTask = 1;

//__THERSHOLDS__
float v_temperatureThreshold = 23; // It will be automated
float v_humidityThreshold = 70;    // It will be automated

//__BUTTON_INCREMENT__
// bool buttonPressed = false;
// int lastButtonState = LOW; // Add this line at the top of your code
// static int b_buttonIncrement = 0; // cu variablia asta vom face switch intre ecrane
// /*************************************************/
// int button_up_clicked = 0; // only perform action when button is clicked, and wait until another press
// /*************************************************/
// /* TEST CU INTRERUPERE */

// volatile uint8_t isr_counter = 0;
// /*************************************************/
// #define DEBOUNCE_DELAY 100 // define debounce time in milliseconds

// unsigned long lastDebounceTime_1 = 0; // the last time the button_1 was pressed
// unsigned long lastDebounceTime_2 = 0; // the last time the button_2 was pressed

//___________________TASK_HANDLERS______________________________//

TaskHandle_t h_dhtReadTask;
TaskHandle_t h_displayStateTask;
TaskHandle_t h_dhtDisplayTask;
TaskHandle_t h_fanControlTask;
TaskHandle_t h_heatControlTask;
TaskHandle_t h_mpu6050Task;
TaskHandle_t h_NTPClockTask;
// TaskHandle_t h_oledDisplayTask;

// SemaphoreHandle_t buttonSemaphore = NULL;

// void IRAM_ATTR buttonPressISR() {
//   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//   xSemaphoreGiveFromISR(buttonSemaphore, &xHigherPriorityTaskWoken);
//   if (xHigherPriorityTaskWoken) {
//     portYIELD_FROM_ISR();
//   }
//   buttonPressed = true;
//   isr_counter ++;
//   if (isr_counter == 4)
//   {
//       isr_counter == 0;
//   }

// }

//____________________________SETUP_____________________________//

void setup()
{
  //__SERIAL_BEGIN__
  Serial.begin(115200); // Setting the Baud Rate and Serial.begin()
  Serial.println("<0> SERIAL BEGIN <1>");
  delay(50);

  Serial.print("<INFO> setup() running on core ");
  Serial.println(xPortGetCoreID());
  delay(500);

  //__PINMODE_INIT_START__
  Serial.println("<*> PINMODE INIT <*>");

  pinMode(p_dhtPin, INPUT);
  Serial.print("  >> pinMode: dhtPin:");
  Serial.print(p_dhtPin);
  Serial.println(" as INPUT.");
  delay(25);

  pinMode(p_heatRelayPin, OUTPUT);
  Serial.print("  >> pinMode: heatRelayPin:");
  Serial.print(p_heatRelayPin);
  Serial.println(" as OUTPUT.");
  delay(25);

  pinMode(p_fanRelayPin, OUTPUT);
  Serial.print("  >> pinMode: fanRelayPin:");
  Serial.print(p_fanRelayPin);
  Serial.println(" as OUTPUT.");
  delay(25);

  Serial.print("  >> pinModes: taskxPrioLED");
  pinMode(p_task1PrioLED, OUTPUT);
  pinMode(p_task2PrioLED, OUTPUT);
  pinMode(p_task3PrioLED, OUTPUT);
  pinMode(p_task4PrioLED, OUTPUT);
  pinMode(p_task5PrioLED, OUTPUT);
  Serial.println(" as OUTPUT.");
  delay(25);

  Serial.print("  >> pinModes: actOnLED");
  pinMode(p_HeatingLED, OUTPUT);
  pinMode(p_CoolingLED, OUTPUT);
  pinMode(p_OffLED, OUTPUT);
  pinMode(UP_LED_PIN, OUTPUT);
  Serial.println(" as OUTPUT.");
  delay(25);

  Serial.print("  >> pinModes: buttonPins");
  // pinMode(p_button_2, INPUT);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  Serial.println(" as INPUT.");
  delay(25);

  //__PINMODE_INIT_END__
  Serial.println("<**> PINMODE INIT FINISHED<**>");
  delay(50);

  //__DHT_22_INIT__
  Serial.println("<*> DHT22 INIT <*>");
  dht.begin();
  delay(50);

  Serial.println("<**> DHT22 INIT FINISHED <**>");
  delay(50);

  //_WIRE_I2C_BEGIN__
  Wire.begin();

  /*******************************************************************/
  //__OLED_DISPLAY_INIT__
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("DISPLAY ON!");
  display.display();
  /*******************************************************************/
  //__LCD_DISPLAY_INIT__
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Smart Home");
  LCD.setCursor(0, 1);
  LCD.print("by Dan C.");
  /*******************************************************************/
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  /*******************************************************************/
  while (!mpu.begin())
  {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }
  Serial.println("MPU6050 ready!");
  /*******************************************************************/
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  //__CREATING_TASKS__
  Serial.println("<*> CREATING THE TASKS <*>");

  xTaskCreatePinnedToCore(
    T_dhtReadTask,  /* Task function. */
    "dhtReadTask",  /* String with name of task. */
    2000,           /* Stack size in bytes. */
    NULL,           /* Parameter passed as input of the task */
    p_dhtReadTask,  /* Priority of the task. */
    &h_dhtReadTask, /* Task handle to keep track of created task */
    0               /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: dhtReadTask");
  delay(25);

  xTaskCreatePinnedToCore(
    T_dhtDisplayTask,  /* Task function. */
    "dhtDisplayTask",  /* String with name of task. */
    2000,              /* Stack size in bytes. */
    NULL,              /* Parameter passed as input of the task */
    p_dhtDisplayTask,  /* Priority of the task. */
    &h_dhtDisplayTask, /* Task handle to keep track of created task */
    0                  /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: dhtDisplayTask");
  delay(25);

  xTaskCreatePinnedToCore(
    T_fanControlTask,  /* Task function. */
    "fanControlTask",  /* String with name of task. */
    2000,              /* Stack size in bytes. */
    NULL,              /* Parameter passed as input of the task */
    p_fanControlTask,  /* Priority of the task. */
    &h_fanControlTask, /* Task handle to keep track of created task */
    0                  /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: fanControlTask");
  delay(25);

  xTaskCreatePinnedToCore(
    T_heatControlTask,  /* Task function. */
    "heatControlTask",  /* String with name of task. */
    2000,               /* Stack size in bytes. */
    NULL,               /* Parameter passed as input of the task */
    p_heatControlTask,  /* Priority of the task. */
    &h_heatControlTask, /* Task handle to keep track of created task */
    0                   /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: heatControlTask");
  delay(25);

  xTaskCreatePinnedToCore(
    T_displayStateTask,  /* Task function. */
    "displayStateTask",  /* String with name of task. */
    2000,                /* Stack size in bytes. */
    NULL,                /* Parameter passed as input of the task */
    p_displayStateTask,  /* Priority of the task. */
    &h_displayStateTask, /* Task handle to keep track of created task */
    0                    /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: displayStateTask");
  delay(25);

  xTaskCreatePinnedToCore(
    T_oledDisplayTask,  /* Task function. */
    "oledDisplayTask",  /* String with name of task. */
    2000,               /* Stack size in bytes. */
    NULL,               /* Parameter passed as input of the task */
    p_oledDisplayTask,  /* Priority of the task. */
    &h_oledDisplayTask, /* Task handle to keep track of created task */
    0                   /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: oledDisplayTask");
  delay(25);

  xTaskCreatePinnedToCore(
    T_mpu6050Task,      /* Task function. */
    "mpu6050Task",      /* String with name of task. */
    2000,               /* Stack size in bytes. */
    NULL,               /* Parameter passed as input of the task */
    p_mpu6050Task,      /* Priority of the task. */
    &h_mpu6050Task,     /* Task handle to keep track of created task */
    0                   /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: mpu6050Task");
  delay(25);

  xTaskCreatePinnedToCore(
    T_NTPClockTask,      /* Task function. */
    "NTPClockTask",      /* String with name of task. */
    2000,               /* Stack size in bytes. */
    NULL,               /* Parameter passed as input of the task */
    p_NTPClockTask,      /* Priority of the task. */
    &h_NTPClockTask,     /* Task handle to keep track of created task */
    0                   /* pin task to core 0 */
  );
  Serial.println("  >> Created Task: NTPClockTask");
  delay(25);


  Serial.println("<**> TASKS CREATED <**>");
  delay(50);

  //__SETUP_FINISHED__
  Serial.println("<!> SETUP FINISHED <!>");
  LCD.clear();
  delay(50);
}

//_____________________________LOOP_____________________________//

void loop()
{
  // int buttonState = digitalRead(BUTTON_GPIO);
  // if (buttonState == HIGH) {
  //   // Execută o acțiune când butonul este apăsat
  //   doSomething();
  // }
  // else if (buttonState == LOW)
  // {
  //   Serial.println("Button Low");
  // }
  // delay(1000);
}

//______________________________________________________________//
//____________________________FUNCTIONS_________________________//
//______________________________________________________________//

//_______________________DISPLAY_STATE_TASK______________________//

void T_displayStateTask(void *parameter) // Displays the current state of the system (num & char)
{
  for (;;)
  {
    digitalWrite(p_task2PrioLED, HIGH);
    // delay(25);

    // Serial.print("[#] displayStateTask():");
    // Serial.print(" prio: ");
    // Serial.print(p_displayStateTask);
    // Serial.print(" | ");
    // Serial.print(" <!> CURRENT STATE:");
    // Serial.print(currentState);
    // Serial.print(" : ");
    // Serial.println(stateToString(currentState));
    // printTaskStackUsage("T_displayStateTask");
    digitalWrite(p_task2PrioLED, LOW);
    // delay(25);

    vTaskDelay(pdMS_TO_TICKS(task_delay * 5)); // Delay for 1000 milliseconds
  }
}

//____________________DHT_READ_&_DISPLAY_TASKS__________________//

void T_dhtReadTask(void *parameter) // Reads data from the sensor. It's callable in the other tasks
{

  for (;;)
  {
    digitalWrite(p_task1PrioLED, HIGH);
    // delay(25);

    v_temperatureValue = dht.readTemperature();
    v_humidityValue = dht.readHumidity();

    // if (isnan(v_humidityValue) || isnan(v_temperatureValue))   // Check if any reads failed and exit early (to try again)
    // {
    //   Serial.println("[#] dhtReadTask() : Failed to read from DHT sensor!");
    //   return;
    // }
    // else
    // {
    //   Serial.print("[#] dhtReadTask(): ");
    //   Serial.print(" prio: ");
    //   Serial.print(p_dhtReadTask);
    //   Serial.print(" | ");
    //   Serial.println(" -> Running OK ");
    // }
    // printTaskStackUsage("T_dhtReadTask");
    digitalWrite(p_task1PrioLED, LOW);
    // delay(25);

    vTaskDelay(pdMS_TO_TICKS(task_delay * 10)); // Delay for 1000 milliseconds
  }
}

void T_dhtDisplayTask(void *parameter) // Displays data from the sensor. Calls dhtReadTask()
{
  for (;;)
  {
    digitalWrite(p_task3PrioLED, HIGH);
    // delay(25);

    // Serial.print("[#] dhtDisplayTask(): ");
    // Serial.print(" prio: ");
    // Serial.print(p_dhtDisplayTask);
    // Serial.print(" | ");

    // Serial.print(" -> Temp: ");
    // Serial.print(v_temperatureValue);
    // Serial.print(" C* | ");

    // Serial.print("Hum: ");
    // Serial.print(v_humidityValue);
    // Serial.println(" % ");
    // printTaskStackUsage("T_dhtDisplayTask");
    digitalWrite(p_task3PrioLED, LOW);
    // delay(25);

    vTaskDelay(pdMS_TO_TICKS(task_delay * 5)); // Delay for 1000 milliseconds
  }
}

//_______________________FAN_CONTROL_TASKS______________________//

//__TASK__
void T_fanControlTask(void *parameter) // Controls the Fan. Calls dhtReadTask() & toggleFan(bool state). Can change the currentState between OFF <--> COOLING
{
  for (;;)
  {
    digitalWrite(p_task4PrioLED, HIGH);
    // delay(25);

    // Serial.print("[#] fanControlTask(): ");
    // Serial.print(" prio: ");
    // Serial.print(p_fanControlTask);
    // Serial.print(" | ");

    fanOnSignal();

    if (!f_fanControlTask)
    {
      f_fanControlTask = true;

      switch (currentState)
      {
        case OFF:
          if ((v_temperatureValue > v_temperatureThreshold))
          {
            currentState = COOLING;
            toggleFan(true);
          }
          break;

        case COOLING:
          if (v_temperatureValue <= v_temperatureThreshold)
          {
            currentState = OFF;
            toggleFan(false);
          }
          break;
      }
      f_fanControlTask = false;
    }
    // printTaskStackUsage("T_fanControlTask");
    digitalWrite(p_task4PrioLED, LOW);
    // delay(25);

    vTaskDelay(pdMS_TO_TICKS(task_delay * 2)); // Delay for 1000 milliseconds
  }
}

//__TOGGLE__
void toggleFan(bool state) // Toggles the FAN --> ON/OFF
{
  digitalWrite(p_fanRelayPin, state ? HIGH : LOW);
  // Serial.println(state ? "Fan is ON" : "Fan is OFF");
}

//__FAN_ON_SIGNAL__
void fanOnSignal() // Signals the currentState for Fan Control
{
  if (currentState == COOLING)
  {
    digitalWrite(p_CoolingLED, HIGH);
    digitalWrite(p_OffLED, LOW);
    // Serial.println(" -> FAN: ON");
  }
  else if (currentState == OFF)
  {
    digitalWrite(p_OffLED, HIGH);
    digitalWrite(p_CoolingLED, LOW);
    // Serial.println(" -> FAN: OFF");
  }
  else
  {
    digitalWrite(p_CoolingLED, LOW);
    // Serial.println(" -> FAN: OFF");
  }
}

//_______________________HEAT_CONTROL_TASKS_____________________//

//__TASK__
void T_heatControlTask(void *parameter) // Controls the Fan. Calls dhtReadTask() & toggleHeat(bool state). Can change the currentState between OFF <--> HEATING
{
  for (;;)
  {
    digitalWrite(p_task5PrioLED, HIGH);
    // delay(25);

    // Serial.print("[#] heatControlTask(): ");
    // Serial.print(" prio: ");
    // Serial.print(p_heatControlTask);
    // Serial.print(" | ");

    heatOnSignal();

    if (!f_heatControlTask)
    {
      f_heatControlTask = true;

      switch (currentState)
      {
        case OFF:
          if (v_temperatureValue < v_temperatureThreshold)
          {
            currentState = HEATING;
            toggleHeat(true);
          }
          break;
        case HEATING:
          if (v_temperatureValue >= v_temperatureThreshold)
          {
            currentState = OFF;
            toggleHeat(false);
          }
          break;
      }
      f_heatControlTask = false;
    }
    // printTaskStackUsage("T_heatControlTask");
    digitalWrite(p_task5PrioLED, LOW);
    // delay(25);

    vTaskDelay(pdMS_TO_TICKS(task_delay * 2)); // Delay for 1000 milliseconds
  }
}

//__TOGGLE__
void toggleHeat(bool state) // Toggles the HEAT --> ON/OFF
{
  digitalWrite(p_heatRelayPin, state ? HIGH : LOW);
  // Serial.println(state ? "Heat is ON" : "Heat is OFF");
}

//__HEAT_ON_SIGNAL__
void heatOnSignal() // Signals the currentState for Heat Control
{

  if (currentState == HEATING)
  {
    digitalWrite(p_HeatingLED, HIGH);
    digitalWrite(p_OffLED, LOW);
    // Serial.println("-> HEAT: ON");
  }
  else if (currentState == OFF)
  {
    digitalWrite(p_OffLED, HIGH);
    digitalWrite(p_HeatingLED, LOW);
    // Serial.println("-> HEAT: OFF");
  }
  else
  {
    digitalWrite(p_HeatingLED, LOW);
    // Serial.println("-> HEAT: OFF");
  }
}


//_______________________MPU_CONTROL_TASK_____________________//
void T_mpu6050Task(void *parameter) // Displays the current state of the system (num & char)
{
  sensors_event_t event;

  for (;;)
  {
    mpu.getAccelerometerSensor()->getEvent(&event);

    // Serial.print("[");Serial.print(millis());Serial.print("] X: ");Serial.print(event.acceleration.x);
    // Serial.print(", Y: ");Serial.print(event.acceleration.y);
    // Serial.print(", Z: ");Serial.print(event.acceleration.z);
    // Serial.println(" m/s^2");
    vTaskDelay(pdMS_TO_TICKS(task_delay * 5)); // Delay for 1000 milliseconds
  }
}

//_______________________NTP_CLOCK_TASK_____________________//
void T_NTPClockTask(void *parameter) // Displays the current state of the system (num & char)
{

  for (;;)
  {
    printLocalTime();
    vTaskDelay(pdMS_TO_TICKS(task_delay * 5)); // Delay for 1000 milliseconds
  }
}

void printLocalTime() 
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) 
  {
    LCD.setCursor(0, 1);
    LCD.println("Connection Err");
    return;
  }

  LCD.setCursor(8, 0);
  LCD.println(&timeinfo, "%H:%M:%S");

  LCD.setCursor(0, 1);
  LCD.println(&timeinfo, "%d/%m/%Y   %Z");
}
/*###############################################################################*/
/*                               AICI LUCREZ AZI                                 */
/*###############################################################################*/
//_____________________NEW_OLED_DISPLAY_TASK________________________//
void T_oledDisplayTask(void *parameter)
{
  for (;;)
  {

    /************************************************************/
    volatile uint8_t oled_screen_number;
    oled_screen_number = get_button_increment();
    change_display_information(oled_screen_number);
    /************************************************************/

    Serial.print("Display count: "); Serial.println(display_increment);

    vTaskDelay(pdMS_TO_TICKS(50)); // Delay for 100 milliseconds

  }
}

void oled_display_update(String oled_message, uint8_t cursor_1, uint8_t cursor_2, uint8_t text_size)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(cursor_1, cursor_2);
  display.println(oled_message);
  display.display();

}


void change_display_information(uint8_t display_index)
{

  String oled_message; // Declare here
  String tempString; // Declare here
  String humidityString; // Declare here
  String wifiNetoworkIP;

  switch (display_index)
  {
    case 0:
      currentOledState = ECRAN_0;
      if (currentOledState == ECRAN_0)
      {
        String title = String(stateToString_oledState(currentOledState)); // Convert to String and remove the state number        int spaces = (20 - title.length()) / 2; // Assuming the display is 20 characters wide
        int spaces = (6 - title.length()) / 2; // Assuming the display is 20 characters wide
        oled_message = String(' ', spaces) + "  " + title +
                       "\n   Smart home" +
                       "\n   by Dan Codreanu";
        oled_display_update(oled_message, 0, 0, 1);
        break;
      }
    case 1:
      currentOledState = ECRAN_1;
      wifiNetoworkIP = WiFi.localIP().toString();
      if (currentOledState == ECRAN_1)
      {
        String title = String(stateToString_oledState(currentOledState)); // Convert to String and remove the state number        int spaces = (20 - title.length()) / 2; // Assuming the display is 20 characters wide
        int spaces = (6 - title.length()) / 2; // Assuming the display is 20 characters wide
        oled_message = String(' ', spaces) + "  "  + title +
                       "\n   Wifi: " + wifiNetoworkIP +
                       "\n   MQTT: " +
                       "\n   BLE:";
        oled_display_update(oled_message, 0, 0, 1);
      }
      break;
    case 2:
      tempString        = String(v_temperatureValue, 1);  // One decimal place
      humidityString    = String(v_humidityValue, 1); // One decimal place
      currentOledState  = ECRAN_2;
      if (currentOledState == ECRAN_2)
      {
        String title = String(stateToString_oledState(currentOledState)); // Convert to String and remove the state number        int spaces = (20 - title.length()) / 2; // Assuming the display is 20 characters wide
        int spaces = (6 - title.length()) / 2; // Assuming the display is 20 characters wide
        oled_message = String(' ', spaces) + "  "  + title +
                       "\nTemp: "         + tempString + "C" +
                       "\nHumidity: "     + humidityString + "%" +
                       "\nCurrent State:" + stateToString(currentState);
        oled_display_update(oled_message, 0, 0, 1);
      }
      break;
    case 3:
      currentOledState = ECRAN_3;
      if (currentOledState == ECRAN_3)
      {
        String title = String(stateToString_oledState(currentOledState)).substring(0); // Convert to String and remove the state number        int spaces = (20 - title.length()) / 2; // Assuming the display is 20 characters wide
        int spaces = (6 - title.length()) / 2; // Assuming the display is 20 characters wide
        oled_message = String(' ', spaces) + "  "  + title +
                       "\nIAQ: " +
                       "\nIAQ: ";
        oled_display_update(oled_message, 0, 0, 1);
      }
      break;
  }
}


int get_button_increment()
{
  check_display_count = manage_button_pressing();
  if ( check_display_count == 1)
  {

    display_increment += 1;

    if (display_increment == 4)
    {
      display_increment = 0;
    }

  }

  return display_increment;
}


int manage_button_pressing()
{
  // Up button
  if ((digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 0))
  {
    button_up_clicked = 1;
    // Serial.println("Up button pressed");
    digitalWrite(UP_LED_PIN, HIGH);
  }
  else if (digitalRead(BUTTON_UP_PIN) == HIGH)
  {
    button_up_clicked = 0;
    // Serial.println("Up button released");
    digitalWrite(UP_LED_PIN, LOW);
  }

  return button_up_clicked;
}




