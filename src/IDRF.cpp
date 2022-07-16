/*
   Software desenvolvido para controle de abertura/fechamento de
   portão eletronico pelo smartphone usando a plataforma Blynk

   Software desenvolvido para controlar um robo suspenso em um cabo


*/

#include "prototypes.h"

bool bOffLineMode = false;
bool f_SerialOut_ON = true;

int intLedOn = LED_STATICO_ON;  // tempo em que o led de status fica on
int intLedOff = LED_STATICO_ON; // tempo em que o led de status fica off
bool bLedState = false;         // indica se o led esta on/off

struct Verify wifi;
struct Verify blynk;
struct ESP_32 MyESP32;
struct RoboCan MyMoves;

unsigned long ulTimeVerify = 0; // variavel de controle

bool bBlynkButtonState = false;
bool bKeyPulseState = false;
bool bReleState = false;

bool lock_button = true;

const char esp32_rede[20] = DEVICE_NAME;
const char esp32_pass[20] = DEVICE_PASS;

char blynk_token[40] = "YOUR_BLYNK_TOKEN";
char blynk_server[25] = "YOUR_SERVER";
char blynk_port[10] = "PORT";

hw_timer_t *timer = NULL;

bool bTouchState = false;
bool f_Reconnect_WIFI = false;
bool f_Reconnect_Blynk = false;

unsigned long TimeCheck = 0;
unsigned long TimeOutConnect = 0;
unsigned long TimeOut = 0;
unsigned long CountTime = 0;

String output2State;

// Servo servoPan;
// Servo servoTilt;

WidgetTerminal terminal(V4);
SSD1306 display(ADDR_I2C_DISPLAY, SDA_PIN, SCL_PIN); // ADDRESS, SDA, SCL

void setup()
{

  Comunication(BAUD_RATE);

  display.init();
  display.clear();
  display.flipScreenVertically(); // inveter
  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 0, "IDRF V1.0");

  MRFC522_setup();

  Setting_Pins();

  Start_Timer(TIME_INTERRUPT);

  display.drawString(10, 16, "Connect WiFi..");
  display.display();

  CONNECTION_WiFi(ATTEMPTS);

  display.drawString(10, 32, "Connected WiFi!");
  display.display();
  BLYNK_connection(ATTEMPTS);

  display.drawString(10, 48, "Connected Blynk!");
  display.display();

  Programming_OTA();

  TimeCheck = TIME_CHECK_CONNECTION;
}

void loop()
{

  // MRFC522_get_id();

  // faz o dispositivo ficar desconectando
  //  if (MyESP32.CheckConnection)
  //  {
  //    CONNECTION_reconnect(ATTEMPTS);
  //    BLYNK_reconnect(ATTEMPTS);
  //    MyESP32.CheckConnection = false;
  //  }

  MRFC522_setup();

  Blynk.run();

  ArduinoOTA.handle();

  delay(1500);

  /* deep_sleep mode */
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // esp_deep_sleep_start();

  /* light sleep mode*/
  // esp_sleep_enable_timer_wakeup(1000000); // 1 second
  // int ret = esp_light_sleep_start();
  // Serial.printf("lp: %d\n", ret);
}

BLYNK_WRITE(UNLOCK_PORT)
{
  int value = param.asInt(); // Get value as integer
  if (value)
  {
    digitalWrite(PIN_SOLENOIDE, HIGH);
    delay(2000);
    digitalWrite(PIN_SOLENOIDE, LOW);
  }
}

BLYNK_READ(GET_VBAT)
{
  //   #define ADC_VBAT 13
  // #define RES_R1 47000.0
  // #define RES_R2 10000.0
  // #define VREF 3.30
  // #define RESOLUTION_ADC 4095
  // Serial.println("*** Read request from Blynk ***");
  unsigned int samples = SAMPLES;
  float potValue = 0;

  while (samples--)
  {
    potValue += analogRead(ADC_VBAT);
  }

  potValue = potValue / SAMPLES;
  // Serial.println("Bateria RAW: " + (String)potValue);
  potValue = (potValue * VREF) / RESOLUTION_ADC;
  // Serial.println("Bateria RAW: " + (String)potValue);
  potValue = potValue * ((RES_R1 + RES_R2) / RES_R2);
  Serial.println("Bateria: " + (String)potValue + "V");
  Blynk.virtualWrite(GET_VBAT, potValue);
}

// BLYNK_WRITE(MOV_CAN_TILT)
// {
//   MyMoves.movCanTilt = true;
//   MyMoves.valueCanTilt = param.asInt();
// }

// BLYNK_WRITE(MOV_ROBO_RIGHT)
// {
//   MyMoves.movRoboRight = true;
//   MyMoves.valueRoboRight = param.asInt();
// }

// BLYNK_WRITE(MOV_ROBO_LEFT)
// {
//   MyMoves.movRoboLeft = true;
//   MyMoves.valueRoboLeft = param.asInt();
// }

BLYNK_WRITE(RESET_WIFI_V50)
{
  int value = param.asInt(); // Get value as integer
  if (value)
  {
    WiFiManager wifiConnect;
    wifiConnect.resetSettings();
    WiFi.disconnect();
    TimeCheck = 1;
    SPIFFS.format();
    ESP.restart();
  }
}
