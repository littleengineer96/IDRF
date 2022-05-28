/*
   Software desenvolvido para controle de abertura/fechamento de
   portão eletronico pelo smartphone usando a plataforma Blynk

   Software desenvolvido para controlar um robo suspenso em um cabo

   ESP-01  IDE
   GPIO0    0
   TXD      1
   GPIO2    2
   RXD      3
*/
#include <Arduino.h>
#include <TimeLib.h>
#include <BlynkSimpleEsp32.h>
// #include <BlynkSimpleEsp8266.h>

#include <WidgetRTC.h>

//#include <ModbusIP_ESP8266.h>
#include <DNSServer.h>

// #include <ESP8266WebServer.h>

#include <connection.h>
#include <globaldef.h>
// #include <blynkLED.h>
#include <memoriaFlash.h>
#include <setupOTA.h>
#include <setupBlynk.h>
#include <saveStatus.h>
#include <isr.h>
#include <setting.h>
#include <prototypes.h>

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
char blynk_token[34] = "YOUR_BLYNK_TOKEN";

hw_timer_t *timer = NULL;

bool bTouchState = false;
bool f_Reconnect_WIFI = false;
bool f_Reconnect_Blynk = false;

unsigned long TimeCheck = 0;
unsigned long TimeOutConnect = 0;
unsigned long TimeOut = 0;
// void inite(uint8_t chg_pin);

Servo servoPan;
Servo servoTilt;

WidgetTerminal terminal(V4);

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup()
{
  Comunication(BAUD_RATE);

  Setting_Pins();

  engine.init();
  stepper = engine.stepperConnectToPin(OUTPIN_A4988_STEP);

  if (stepper)
  {
    stepper->setDirectionPin(OUTPIN_A4988_DIR);
    stepper->setEnablePin(OUTPIN_A4988_EN);
    stepper->setAutoEnable(true);

    // If auto enable/disable need delays, just add (one or both):
    // stepper->setDelayToEnable(50);
    // stepper->setDelayToDisable(1000);

    stepper->setSpeedInUs(1000); // the parameter is us/step !!!
    stepper->setAcceleration(100);
    stepper->move(1000);
  }

  Start_Timer(TIME_INTERRUPT);

  MyESP32.ConnectedWiFi = Assistant_WiFi();

  if (MyESP32.ConnectedWiFi)
  {
    int result = Assistant_Blynk();
    if (result == SUCCESS_)
    {
      MyESP32.ConnectedBlynk = true;
    }
    else if ((result == ERRO_) || result == INVALID_TOKEN)
    {
      MyESP32.ConnectedBlynk = false;
    }
    else
    {
      MyESP32.ConnectedBlynk = false;
      MyESP32.ConnectedWiFi = false;
    }
  }

  Programming_OTA();

  TimeCheck = TIME_CHECK_CONNECTION;
}

void loop()
{
  if (MyESP32.CheckConnection)
  {
    MyESP32.ConnectedBlynk = Blynk.connected();
    MyESP32.ConnectedWiFi = WiFi_Connected();

    if (!MyESP32.ConnectedWiFi)
    {
      MyESP32.ConnectedWiFi = Assistant_WiFi();
    }

    if (!MyESP32.ConnectedBlynk && MyESP32.ConnectedWiFi)
    {
      int result = Assistant_Blynk();
      if (result == SUCCESS_)
      {
        MyESP32.ConnectedBlynk = true;
      }
      else if ((result == ERRO_) || result == INVALID_TOKEN)
      {
        MyESP32.ConnectedBlynk = false;
      }
      else
      {
        MyESP32.ConnectedBlynk = false;
        MyESP32.ConnectedWiFi = false;
      }
    }

    MyESP32.CheckConnection = false;
    TimeCheck = TIME_CHECK_CONNECTION; // reload timer
    // Serial.println(".");
  }

  if (MyESP32.ConnectedBlynk)
  {
    Blynk.run();
  }

  if (MyMoves.movCanPan)
  {
    MyMoves.movCanPan = false;
    Serial.println("Movendo Camera (PAN) value: " + String(MyMoves.valueCanPan));
    servoPan.write(MyMoves.valueCanPan);
  }

  if (MyMoves.movCanTilt)
  {
    MyMoves.movCanTilt = false;
    Serial.println("Movendo Camera (TILT) value: " + String(MyMoves.valueCanTilt));
    servoTilt.write(MyMoves.valueCanTilt);
  }

  if (MyMoves.movRoboRight)
  {
    MyMoves.movRoboRight = false;
    Serial.println("Movendo Robo (RIGHT) value: " + String(MyMoves.valueRoboRight));
  }

  if (MyMoves.movRoboLeft)
  {
    MyMoves.movRoboLeft = false;
    Serial.println("Movendo Robo (LEFT) value: " + String(MyMoves.valueRoboLeft));
  }

  ArduinoOTA.handle();
}

BLYNK_WRITE(MOV_CAN_PAN)
{
  MyMoves.movCanPan = true;
  MyMoves.valueCanPan = param.asInt();
}

BLYNK_WRITE(MOV_CAN_TILT)
{
  MyMoves.movCanTilt = true;
  MyMoves.valueCanTilt = param.asInt();
}

BLYNK_WRITE(MOV_ROBO_RIGHT)
{
  MyMoves.movRoboRight = true;
  MyMoves.valueRoboRight = param.asInt();
}

BLYNK_WRITE(MOV_ROBO_LEFT)
{
  MyMoves.movRoboLeft = true;
  MyMoves.valueRoboLeft = param.asInt();
}

BLYNK_WRITE(RESET_WIFI_V255)
{
  int value = param.asInt(); // Get value as integer
  if (value)
  {
    WiFiManager wifiConnect;
    wifiConnect.resetSettings();
    WiFi.disconnect();
    TimeCheck = 1;
    // SPIFFS.format();
    // ESP.restart();
  }
}
