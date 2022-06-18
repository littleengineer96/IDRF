#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "prototypes.h"

extern bool bOffLineMode;
extern bool f_SerialOut_ON;

extern int intLedOn;
extern int intLedOff;
extern bool bLedState;

extern struct Verify wifi;
extern struct Verify blynk;
extern struct ESP_32 MyESP32;
extern struct RoboCan MyMoves;

extern unsigned long ulTimeVerify;

extern bool bBlynkButtonState;
extern bool bKeyPulseState;
extern bool bReleState;

extern bool lock_button;

extern const char esp32_rede[20];
extern const char esp32_pass[20];
// extern char blynk_token[34];

extern hw_timer_t *timer;

extern bool bTouchState;
extern bool f_Reconnect_WIFI;
extern bool f_Reconnect_Blynk;

extern unsigned long TimeCheck;
extern unsigned long TimeOutConnect;
extern unsigned long TimeOut;
extern unsigned long CountTime;

// void inite(uint8_t chg_pin);

extern String output2State;

// extern Servo servoPan;
// extern Servo servoTilt;

// extern WidgetTerminal terminal();

// extern FastAccelStepperEngine engine;
// extern FastAccelStepper *stepper;

#endif