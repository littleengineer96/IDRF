#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include "prototypes.h"

#define BLYNK_PRINT Serial
#define BAUD_RATE 115200
#define DATA_STATE_ADDRESS "/dataState.txt"

#define LED_PIN 2   // TXD
#define RELE_PIN 13 // GPIO02
//#define KEY_PULSE_PIN 3   //RXD
#define TOUCH_PIN 14 // pin touch
//#define CHARGE_PIN 2 //pin charge
#define CHARGE_PIN 15 // pin charge

#define FATOR_TCH 2000

#define N_VERIFY 10 // intervalo de verificações Blynk.connected();

#define VERSION "1v0-"         // versão 1.0
#define SERIE "1"              // serie 2
#define LOTE "A"               // lote "A"
#define DEVICE_NAME "IDRF_1v0" // nome completo do dispositivo
#define DEVICE_PASS "12345678"
//#define DIVICE_NAME "WiGatekey2030" //nome completo do dispositivo

#define ssidAP DEVICE_NAME
#define passAP DEVICE_PASS

#define TIMEOUT_PORTAL 60      // time in s
#define TIME_INTERRUPT 1000000 // tempo em us
//#define TIME_ISR_RESET 500 //tempo em milisegundos, deve ser 1000 para que TIMER_AP seja inserido em segundos
//#define TIMER_AP 150 //tempo  1s = 2   150 = 75s
#define TIME_ISR_RESET 250 // tempo em milisegundos, deve ser 1000 para que TIMER_AP seja inserido em segundos
#define TIMER_AP 150       // tempo  1s = 2   150 = 75s
#define SIZE_TOKEN 32      // tamanho do token para verificação
#define SIZE_SERVER 17     // tamanho do token para verificação
#define SIZE_PORT 5        // tamanho do token para verificação

#define DELAY 2000         // tempo em ms
#define TIME_LED_ON 500L   // tempo em ms
#define TIME_LED_OFF 2500L // tempo em ms

#define TIME_OUT_BLYNK_CONNECT 800L

// connect
#define SUCCESS 1
#define ERRO 0
#define DEBUG 0
#define TRY_AGAIN_CONNECT 4
#define TIME_CHECK_CONNECTION 30 // SEGUNDOS
#define TIMEOUT_CONNECT 200      //

#define LED_BORD 2
#define ATTEMPTS 3
#define SAMPLES 10

#define ADC_VBAT 34
#define RES_R1 47000.0
#define RES_R2 8290.0
#define VREF 3.30
#define RESOLUTION_ADC 4095

// blynk virtual pins
#define RESET_WIFI_V255 V255 // switch
// #define ON_OFF_V2 V2      // switch
// #define LOCK_BUTTON_V3 V3 // status
#define MOV_CAN_PAN V1
#define MOV_CAN_TILT V2
#define MOV_ROBO_RIGHT V3
#define MOV_ROBO_LEFT V4

#define UNLOCK_PORT V1  // PINO VIRTUAL PARA CONTROLAR SOLENOIDE
#define PIN_SOLENOIDE 4 // PINO DE CONTROLE DE SOLENOIDE
#define GET_VBAT V2

// pins ESP32
#define OUTPIN_SERVO_PAN 25
#define OUTPIN_SERVO_TILT 26
#define OUTPIN_A4988_EN 27
#define OUTPIN_A4988_SLEEP 14
#define OUTPIN_A4988_STEP 12
#define OUTPIN_A4988_DIR 16

#define PIN_CS_MRFC522 5
#define PIN_RST_MRFC522 2

#define UID_TAG_CARD "C7:CE:CE:D7"
#define UID_TAG_CHAVEIRO "73:39:99:14"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 10        /* Time ESP32 will go to sleep (in seconds) */

enum State
{                           //                                                         LED_ON    LED_OFF
    NOT_CONNECTED_ST,       // acabou de iniciar não conectado. LED STATICO   CONST
    CONNECTING_ST,          // conectando...                    LED RAPIDO    160ms    100ms
    CONNECTED_ST,           // conectado ao Wifi!               LED LENTO     3000ms   500ms
    DISCONNECTED_ST,        // disconexão!                     LED MEDIO     500ms    500ms
    BLYNK_NOT_CONNECTED_ST, // não conectado ao blynk
    BLYNK_CONNECTING_ST,    // conectando ao Blynk
    BLYNK_CONNECTED_ST,     // conectado ao Blynk
    BLYNK_DISCONNECTED_ST   // desconectado do Blynk
};

enum StateVerify
{
    NOT_CONNECTED_VERIFY = 10, //
    CONNECTING_VERIFY = 3,     // quantidade de vezes
    CONNECTION_ATTEMPTS = 3,
    CONNECTED_VERIFY = 15000,    // tempo em ms
    DISCONNECTED_VERIFY = 1,     // quantidade de vezes
    BLYNK_CONNECTING_VERIFY = 3, // quantidade de vezes
    BLYNK_CONNECTION_ATTEMPTS = 3,
    BLYNK_CONNECTED_VERIFY = 15000,        // tempo em ms
    OFF_MODE_BLYNK_CONNECTING_VERIFY = 14, // quantidade de vezes que a conexão com o Blynk é tentada em modo offline 14x15s=210segundos
    BLYNK_DISCONNECTED_VERIFY = 2          // quantidade de vezes
};

enum StateLED
{
    LED_STATICO_ON = 1000,

    LED_RAPIDO_ON = 160,
    LED_RAPIDO_OFF = 100,

    LED_MEDIO_ON = 500,
    LED_MEDIO_OFF = 500,

    LED_LENTO_ON = 500,
    LED_LENTO_OFF = 3000
};

struct Verify
{

    int ConectionState = -1;           // stato da conexao
    int ConnectionAttemptsCounter = 0; // contador de tentativas de conexao

    int ConnectingVerify = 0;     // variavel de controle
    int NotConnectedVerify = 0;   // variavel de controle
    int ConnectedVerify = 0;      // variavel de controle
    int DisconnectedVerify = 0;   // variavel de controle
    unsigned long TimeVerify = 0; // variavel de controle
};

struct ESP_32
{
    int CurrentState = 0; // stato da conexao
    int CurrentAttempts = 0;
    bool ConnectedWiFi = false;
    bool ConnectedBlynk = false;
    bool OfflineMode = false;
    bool Carrying = false;
    bool ReturnDoca = false;
    bool TurnOnCAM = false;
    bool Reconnect = false;
    bool CheckConnection = false;
};

struct RoboCan
{
    bool movCanPan = false;
    int valueCanPan = 0;

    bool movCanTilt = false;
    int valueCanTilt = 0;

    bool movRoboRight = false;
    int valueRoboRight = 0;

    bool movRoboLeft = false;
    int valueRoboLeft = 0;
};

#endif