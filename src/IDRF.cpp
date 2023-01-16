/*
   Software desenvolvido para controle de abertura/fechamento de
   portão eletronico pelo smartphone usando a plataforma Blynk

   Software desenvolvido para controlar um robo suspenso em um cabo


*/

#include "prototypes.h"
#include <Keypad.h> // Biblioteca do codigo

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

String output2State;

// Servo servoPan;
// Servo servoTilt;

WidgetTerminal terminal(V4);

const byte LINHAS = 4;  // Linhas do teclado
const byte COLUNAS = 3; // Colunas do teclado

// const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
//   {'1', '2', '3', 'A'},
//   {'4', '5', '6', 'B'},
//   {'7', '8', '9', 'C'},
//   {'*', '0', '#', 'D'}
// };

// const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
//   {'1', '2', '3', },
//   {'4', '5', '6', },
//   {'7', '8', '9', },
//   {'*', '0', '#', }
// };

// byte PINOS_LINHAS[LINHAS] = {12, 14, 27, 26}; // Pinos de conexao com as linhas do teclado
// byte PINOS_COLUNAS[COLUNAS] = {25, 33, 32}; // Pinos de conexao com as colunas do teclado

// Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); // Inicia teclado

// armazena o valor da leitura inicial
unsigned long startValueTouch12;
unsigned long startValueTouch14;
unsigned long startValueTouch27;

float percentTouch12 = 0.30; // percentual com relação a primeira leitura
float percentTouch14 = 0.30; // percentual com relação a primeira leitura
float percentTouch27 = 0.30; // percentual com relação a primeira leitura

unsigned long factorTouch12;
unsigned long factorTouch14;
unsigned long factorTouch27;

unsigned long touch12;
unsigned long touch14;
unsigned long touch27;
// unsigned long timeOut;

unsigned long debounceTouch12;
unsigned long debounceTouch14;
unsigned long debounceTouch27;

bool leanTouch12 = false; // indica que o Touch foi tocado...
bool leanTouch14 = false; // indica que o Touch foi tocado...
bool leanTouch27 = false; // indica que o Touch foi tocado...
void setup()
{
  Comunication(BAUD_RATE);
  // Serial.println(".");
  pinMode(PIN_SOLENOIDE, OUTPUT);
  MRFC522_setup();

  // Setting_Pins();

  // Start_Timer(TIME_INTERRUPT);
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

  // if (Blynk.connect())
  // {
  //   Serial.println("\nConnected to Blynk!");
  // }
  // else
  // {
  //   Serial.println("\nError Connect to Blynk.");
  // }
  // WL_CONNECTED
  // blynkConnect();

  // Programming_OTA();
  TimeCheck = TIME_CHECK_CONNECTION;

  startValueTouch12 = touchRead(12);
  startValueTouch14 = touchRead(14);
  startValueTouch27 = touchRead(27);

  Serial.println("Leitura inicial");
  Serial.println("Touch 12: " + String(startValueTouch12));
  Serial.println("Touch 14: " + String(startValueTouch14));
  Serial.println("Touch 27: " + String(startValueTouch27));

  factorTouch12 = startValueTouch12 * percentTouch12;
  factorTouch14 = startValueTouch14 * percentTouch14;
  factorTouch27 = startValueTouch27 * percentTouch27;

  Serial.println("Factor Touch 12: " + String(factorTouch12));
  Serial.println("Factor Touch 14: " + String(factorTouch14));
  Serial.println("Factor Touch 27: " + String(factorTouch27) + "\n");
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
  // Serial.println("...");

  // MRFC522_setup();
  MRFC522_get_id();
  // Serial.println("....");

  touch12 = touchRead(12);
  touch14 = touchRead(14);
  touch27 = touchRead(27);

  if ((touch12 < (factorTouch12)) && ((millis() - debounceTouch12) > 200))
  {
    // Serial.println(".");
    leanTouch12 = true;
  }
  if ((touch14 < (factorTouch14)) && ((millis() - debounceTouch14) > 200))
  {
    // Serial.println("..");
    leanTouch14 = true;
  }
  if ((touch27 < (factorTouch27)) && ((millis() - debounceTouch27) > 200))
  {
    // Serial.println("...");
    leanTouch27 = true;
  }

  if ((touch12 > (factorTouch12 * 2)) && leanTouch12)
  {
    leanTouch12 = false;
    Serial.println("Touch 12: " + String(touch12));
    debounceTouch12 = millis();
  }

  if ((touch14 > (factorTouch14 * 2)) && leanTouch14)
  {
    leanTouch14 = false;
    Serial.println("Touch 14: " + String(touch14));
    debounceTouch14 = millis();
  }

  if ((touch27 > (factorTouch27 * 2)) && leanTouch27)
  {
    leanTouch27 = false;
    Serial.println("Touch 27: " + String(touch27));
    debounceTouch27 = millis();
  }

  // verifica a necessidade de uma calibração
  if (touch12 > ((startValueTouch12 * percentTouch12) + startValueTouch12))
  {
    Serial.println("Fazer calibração Touch12");
    startValueTouch12 = touch12;
    factorTouch12 = startValueTouch12 * percentTouch12;
  }

  if (touch14 > ((startValueTouch14 * percentTouch14) + startValueTouch14))
  {
    Serial.println("Fazer calibração Touch14");
    startValueTouch14 = touch14;
    factorTouch14 = startValueTouch14 * percentTouch14;
  }

  if (touch27 > ((startValueTouch27 * percentTouch27) + startValueTouch27))
  {
    Serial.println("Fazer calibração Touch27");
    startValueTouch27 = touch27;
    factorTouch27 = startValueTouch27 * percentTouch27;
  }

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

  // Blynk.run();

  // ArduinoOTA.handle();

  // delay(1000);

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
