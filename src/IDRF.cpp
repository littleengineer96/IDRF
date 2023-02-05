/*
   Software desenvolvido para controle de abertura/fechamento de
   portão eletronico pelo smartphone usando a plataforma Blynk
*/

#include "prototypes.h"
#include <ESPmDNS.h>
// #include "interface.h"
#include "interface2.h"
#include <NTPClient.h>
#include <DallasTemperature.h>

#define BLYNK_TEMPLATE_ID "TMPLYwgbTZS9"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "36FUxqF7OKaLXCVpaqjFIeROeq535M1Q"

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

char blynk_token[35] = "YOUR_BLYNK_TOKEN";
char blynk_server[17] = "YOUR_SERVER";
char blynk_port[5] = "PORT";

hw_timer_t *timer = NULL;

bool bTouchState = false;
bool f_Reconnect_WIFI = false;
bool f_Reconnect_Blynk = false;

unsigned long TimeCheck = 0;
unsigned long TimeOutConnect = 0;
unsigned long TimeOut = 0;
unsigned long CountTime = 0;
unsigned long ledBlynkTime = 0;
unsigned long ledBlynkTimeOff = 0;
unsigned long ledBlynkTimeOn = 0;
bool ledBlynkState = false;
String output2State;

WidgetTerminal terminal(V4);

// WiFiServer server(80);
// WiFiClient client;
AsyncWebServer server(855);

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

const byte LINHAS = 4;  // Linhas do teclado
const byte COLUNAS = 3; // Colunas do teclado

// armazena o valor da leitura inicial
unsigned long startValueTouch12;
unsigned long startValueTouch14;
unsigned long startValueTouch27;

// percentual com relação a primeira leitura
float percentTouch12 = 0.30;
float percentTouch14 = 0.30;
float percentTouch27 = 0.30;

unsigned long factorTouch12;
unsigned long factorTouch14;
unsigned long factorTouch27;

unsigned long touch12;
unsigned long touch14;
unsigned long touch27;

unsigned long debounceTouch12 = 200;
unsigned long debounceTouch14 = 200;
unsigned long debounceTouch27 = 200;

unsigned long timeoutTouch12;
unsigned long timeoutTouch14;
unsigned long timeoutTouch27;

// indica que o Touch foi tocado...
bool leanTouch12 = false;
bool leanTouch14 = false;
bool leanTouch27 = false;

// toque reconhecido
bool recognizedTouch12 = false;
bool recognizedTouch14 = false;
bool recognizedTouch27 = false;

unsigned long timeVerify = 0;
bool doorState = false;
String WiFi_State();
// BlynkTimer timer;
double analogReadAdjusted(byte pinNumber);

const int oneWireBus = 26;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
float readTemp();
float tempCelsius = -1;
bool modeAPOn = false;
void setup()
{
  Comunication(BAUD_RATE);

  // Serial.println(".");
  pinMode(PIN_SOLENOIDE, OUTPUT);
  pinMode(PIN_REEDSWITCH, INPUT_PULLUP);
  pinMode(PIN_LED_BORD, OUTPUT);
  pinMode(PIN_DS18B20, INPUT_PULLUP);
  // ledcSetup(0, 1000, 8);
  // ledcAttachPin(PIN_SOLENOIDE, 0);
  ledBlynkTimeOff = LED_TIME_WIFI_CONNECTING;
  ledBlynkTimeOn = LED_TIME_ON;
  Start_Timer(TIME_INTERRUPT);

  // WiFi.mode(WIFI_MODE_APSTA);
  // WiFi.softAP(SSID_AP, PASS_AP);
  // WiFi.disconnect();
  WiFi.begin(SSID_AP, PASS_AP);
  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");

    if (attempts == 20)
    {
      WiFi.mode(WIFI_MODE_APSTA);
      // WiFi.softAP(SSID_AP, PASS_AP);
      // modeAPOn = true;
      break;
    }

    attempts +=1;
  }
  interface2Begin();

  MRFC522_setup();

  // digitalWrite(PIN_RST_MRFC522, HIGH);

  // Setting_Pins();

  // if (!MDNS.begin("mydevice.com"))
  // {
  //   Serial.print("\nerro ao iniciar mDNS, tentando novamente...");
  // }

  // Serial.println("..");
  // setupWIFI();
  // CONNECTION_WiFi(ATTEMPTS);
  // BLYNK_connection(ATTEMPTS);

  // // Blynk.config(blynk_token, blynk_server, 8080);
  // // Blynk.begin(blynk_token, "Game_Play_LanH", "25061997", IPAddress(104, 154, 136, 221), 8080);
  // // Blynk.config(blynk_token);
  // // Blynk.connect();
  // // Serial.println(WiFi.status());
  // if (WiFi.status() == WL_CONNECTED)
  // {
  //   Serial.println("Conectado ao Wifi");
  // }
  // Blynk.begin(BLYNK_AUTH_TOKEN, SSID_AP, PASS_AP);
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud");
  if (Blynk.connect())
  {
    Serial.println("\nConnected to Blynk!");
  }
  else
  {
    Serial.println("\nError Connect to Blynk.");
  }
  // WL_CONNECTED
  // blynkConnect();

  Programming_OTA();
  TimeCheck = TIME_CHECK_CONNECTION;

  // startValueTouch12 = touchRead(12);
  // startValueTouch14 = touchRead(14);
  // startValueTouch27 = touchRead(27);

  // Serial.println("Leitura inicial");
  // Serial.println("Touch 12: " + String(startValueTouch12));
  // Serial.println("Touch 14: " + String(startValueTouch14));
  // Serial.println("Touch 27: " + String(startValueTouch27));

  // factorTouch12 = startValueTouch12 * percentTouch12;
  // factorTouch14 = startValueTouch14 * percentTouch14;
  // factorTouch27 = startValueTouch27 * percentTouch27;

  // Serial.println("Factor Touch 12: " + String(factorTouch12));
  // Serial.println("Factor Touch 14: " + String(factorTouch14));
  // Serial.println("Factor Touch 27: " + String(factorTouch27) + "\n");

  // server.begin();
  // timeVerify = millis();

  ntp.begin();
  // GMT em segundos
  // +1 = 3600
  // +8 = 28800
  // -1 = -3600
  // -3 = -10800 (BRASIL)
  ntp.setTimeOffset(-10800);

  if (ntp.update())
  {
    Serial.print("DATA/HORA: ");
    Serial.println(ntp.getFormattedDate());

    Serial.print("HORARIO: ");
    Serial.println(ntp.getFormattedTime());

    // Serial.print("HORA: ");
    // Serial.println(ntp.getHours());

    // Serial.print("MINUTOS: ");
    // Serial.println(ntp.getMinutes());

    // Serial.print("SEGUNDOS: ");
    // Serial.println(ntp.getSeconds());

    // Serial.print("DIA DA SEMANA (0=domingo): ");
    // Serial.println(ntp.getDay());

    // Serial.print("EPOCH TIME (Segundos desde 01/01/1970): ");
    // Serial.println(ntp.getEpochTime());
    Serial.println("[NTP] RTC Updated");
  }
  else
  {
    if (!ntp.forceUpdate())
    {
      Serial.println("[NTP] ERROR UPDATE RTC");
    }
  }

  ledBlynkTimeOff = LED_TIME_WIFI_CONNECTED;

  doorState = !digitalRead(PIN_REEDSWITCH);

  Blynk.virtualWrite(V8, WiFi.localIP().toString());
}

void loop()
{
  Blynk.run();

  if (TimeCheck == 0)
  {
    if (!Blynk.connected())
    {
      if (Blynk.connect())
      {
        Serial.println("\nConnected to Blynk!");
      }
      else
      {
        Serial.println("\nError Connect to Blynk.");
      }
    }

    TimeCheck = TIME_CHECK_CONNECTION;
  }

  if (doorState != digitalRead(PIN_REEDSWITCH))
  {
    doorState = digitalRead(PIN_REEDSWITCH);
    String stateDoor = "Porta Aberta";
    if (!doorState)
    {
      stateDoor = "Porta Fechada";
    }
    Blynk.virtualWrite(DOOR_STATE_V2, stateDoor);
    // delay(100);
  }

  // else if (ledBlynkTimeOn == 0)
  // {
  //   digitalWrite(PIN_LED_BORD, false);
  //   ledBlynkTimeOff = LED_TIME_OFF;
  //   ledBlynkTimeOn = LED_TIME_ON;
  // }

  // timer.run();
  // MRFC522_get_id();

  // faz o dispositivo ficar desconectando
  //  if (MyESP32.CheckConnection)
  //  {
  //    CONNECTION_reconnect(ATTEMPTS);
  //    BLYNK_reconnect(ATTEMPTS);
  //    MyESP32.CheckConnection = false;
  //  }
  // Serial.println("...");

  // MRFC522_setup();
  MRFC522_get_id();
  // Serial.println("....");

  // touch12 = touchRead(12);
  // touch14 = touchRead(14);
  // touch27 = touchRead(27);

  // identifica o toque
  // if ((touch12 < (factorTouch12)) && ((millis() - timeoutTouch12) > debounceTouch12))
  // {
  //   leanTouch12 = true;
  // }
  // if ((touch14 < (factorTouch14)) && ((millis() - timeoutTouch14) > debounceTouch14))
  // {
  //   leanTouch14 = true;
  // }
  // if ((touch27 < (factorTouch27)) && ((millis() - timeoutTouch27) > debounceTouch27))
  // {
  //   leanTouch27 = true;
  // }

  // define o toque... "apos levantar o dedo"
  // if ((touch12 > (factorTouch12 * 2)) && leanTouch12)
  // {
  //   leanTouch12 = false;
  //   Serial.println("Touch 12: " + String(touch12));
  //   timeoutTouch12 = millis();
  //   recognizedTouch12 = true;
  // }

  // if ((touch14 > (factorTouch14 * 2)) && leanTouch14)
  // {
  //   leanTouch14 = false;
  //   Serial.println("Touch 14: " + String(touch14));
  //   timeoutTouch14 = millis();
  //   recognizedTouch14 = true;
  // }

  // if ((touch27 > (factorTouch27 * 2)) && leanTouch27)
  // {
  //   leanTouch27 = false;
  //   Serial.println("Touch 27: " + String(touch27));
  //   timeoutTouch27 = millis();
  //   recognizedTouch27 = true;
  // }

  // // verifica a necessidade de uma calibração
  // if (touch12 > ((startValueTouch12 * percentTouch12) + startValueTouch12))
  // {
  //   Serial.println("Fazer calibração Touch12");
  //   startValueTouch12 = touch12;
  //   factorTouch12 = startValueTouch12 * percentTouch12;
  // }
  // if (touch14 > ((startValueTouch14 * percentTouch14) + startValueTouch14))
  // {
  //   Serial.println("Fazer calibração Touch14");
  //   startValueTouch14 = touch14;
  //   factorTouch14 = startValueTouch14 * percentTouch14;
  // }
  // if (touch27 > ((startValueTouch27 * percentTouch27) + startValueTouch27))
  // {
  //   Serial.println("Fazer calibração Touch27");
  //   startValueTouch27 = touch27;
  //   factorTouch27 = startValueTouch27 * percentTouch27;
  // }

  // Serial.println("Touch 26: " + String(touchRead(26)));

  // Serial.println("Touch 25: " + String(touchRead(25)));
  // Serial.println("Touch 33: " + String(touchRead(33)));
  // Serial.println("Touch 32: " + String(touchRead(32)));
  // Serial.println("Touch 35: " + String(touchRead(35)));

  // char leitura_teclas = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado

  // if (leitura_teclas)
  // {                                 // Se alguma tecla foi pressionada
  //   Serial.println(leitura_teclas); // Imprime a tecla pressionada na porta serial
  // }

  ArduinoOTA.handle();

  // delay(1000);

  /* deep_sleep mode */
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // esp_deep_sleep_start();

  /* light sleep mode*/
  // esp_sleep_enable_timer_wakeup(1000000); // 1 second
  // int ret = esp_light_sleep_start();
  // Serial.printf("lp: %d\n", ret);

  // handle_interface();

  if (millis() - timeVerify > 10000)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("SSID: " + WiFi.SSID() + "(" + String(WiFi.RSSI()) + ")");
      Serial.println("State WiFi: " + WiFi_State());
    }

    // Blynk.virtualWrite(DOOR_STATE_V2, stateDoor);

    double Vbat; //= analogRead(ADC_VBAT);
    Vbat = analogReadAdjusted(ADC_VBAT);

    Vbat = (Vbat * VREF) / RESOLUTION_ADC;
    Serial.println("Vin:" + String(Vbat) + " V");

    Vbat = ((RES_R1 + RES_R2) * Vbat) / RES_R2;

    Serial.println("Vbat:" + String(Vbat) + " V");
    // Serial.println("Vbat_:" + String(Vbat) + " V");
    Blynk.virtualWrite(V6, Vbat);

    tempCelsius = readTemp();
    Serial.println("Temp:" + String(tempCelsius) + "oC");

    Blynk.virtualWrite(V7, tempCelsius);

    timeVerify = millis();
  }
}

double analogReadAdjusted(byte pinNumber)
{

  // Specify the adjustment factors.
  const double f1 = 1.7111361460487501e+001;
  const double f2 = 4.2319467860421662e+000;
  const double f3 = -1.9077375643188468e-002;
  const double f4 = 5.4338055402459246e-005;
  const double f5 = -8.7712931081088873e-008;
  const double f6 = 8.7526709101221588e-011;
  const double f7 = -5.6536248553232152e-014;
  const double f8 = 2.4073049082147032e-017;
  const double f9 = -6.7106284580950781e-021;
  const double f10 = 1.1781963823253708e-024;
  const double f11 = -1.1818752813719799e-028;
  const double f12 = 5.1642864552256602e-033;

  // Specify the number of loops for one measurement.
  const int loops = 10;

  // Specify the delay between the loops.
  const int loopDelay = 0;

  // Initialize the used variables.
  int counter = 1;
  int inputValue = 0;
  double totalInputValue = 0;
  double averageInputValue = 0;

  // Loop to get the average of different analog values.
  for (counter = 1; counter <= loops; counter++)
  {

    // Read the analog value.
    inputValue = analogRead(pinNumber);

    // Add the analog value to the total.
    totalInputValue += inputValue;

    // Wait some time after each loop.
    // delay(loopDelay);
  }

  // Calculate the average input value.
  averageInputValue = totalInputValue / loops;

  // Calculate and return the adjusted input value.
  return f1 + f2 * pow(averageInputValue, 1) + f3 * pow(averageInputValue, 2) + f4 * pow(averageInputValue, 3) + f5 * pow(averageInputValue, 4) + f6 * pow(averageInputValue, 5) + f7 * pow(averageInputValue, 6) + f8 * pow(averageInputValue, 7) + f9 * pow(averageInputValue, 8) + f10 * pow(averageInputValue, 9) + f11 * pow(averageInputValue, 10) + f12 * pow(averageInputValue, 11);
}

BLYNK_WRITE(UNLOCK_PORT)
{
  int value = param.asInt(); // Get value as integer
  if (value)
  {
    // digitalWrite(PIN_SOLENOIDE, HIGH);
    // delay(2000);
    // digitalWrite(PIN_SOLENOIDE, LOW);
    MRFC522_open(3000);
  }
}

BLYNK_READ(DOOR_STATE_V2)
{
  //   #define ADC_VBAT 13
  // #define RES_R1 47000.0
  // #define RES_R2 10000.0
  // #define VREF 3.30
  // #define RESOLUTION_ADC 4095
  // Serial.println("*** Read request from Blynk ***");
  // unsigned int samples = SAMPLES;
  // float potValue = 0;

  // while (samples--)
  // {
  //   potValue += analogRead(ADC_VBAT);
  // }

  // potValue = potValue / SAMPLES;
  // // Serial.println("Bateria RAW: " + (String)potValue);
  // potValue = (potValue * VREF) / RESOLUTION_ADC;
  // // Serial.println("Bateria RAW: " + (String)potValue);
  // potValue = potValue * ((RES_R1 + RES_R2) / RES_R2);
  // Serial.println("Bateria: " + (String)potValue + "V");
  int statePint = !digitalRead(PIN_REEDSWITCH);
  Blynk.virtualWrite(DOOR_STATE_V2, statePint);
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

BLYNK_WRITE(RESET_WIFI_V255)
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

String WiFi_State()
{
  switch (WiFi.status())
  {
  case WL_NO_SHIELD:
    return "WL_NO_SHIELD";
    break;
  case WL_IDLE_STATUS:
    return "WL_IDLE_STATUS";
    break;
  case WL_NO_SSID_AVAIL:
    return "WL_NO_SSID_AVAIL";
    break;
  case WL_SCAN_COMPLETED:
    return "WL_SCAN_COMPLETED";
    break;
  case WL_CONNECTED:
    return "WL_CONNECTED";
    break;
  case WL_CONNECT_FAILED:
    return "WL_CONNECT_FAILED";
    break;
  case WL_CONNECTION_LOST:
    return "WL_CONNECTION_LOST";
    break;
  case WL_DISCONNECTED:
    return "WL_DISCONNECTED";
    break;
  default:
    return "WL_ERRO";
  }
}

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

float readTemp()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  return tempC;
}