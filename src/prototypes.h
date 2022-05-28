#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>
#include "FastAccelStepper.h"

// settings
void Setting_Pins();
void Comunication(int baud_rate);
void Input_Pins();
void Output_Pins();
void Initialize_Variables();
void Setting_Servo();

// ISR
void Start_Timer(uint64_t time_in_us);
void My_Timer();

// void saveConfigCallback();
// void MyWiFi();

// connection
int Assistant_WiFi();
int WiFi_Manager();
int Reconnect_WiFi();
int Blynk_Connect();
String WiFi_State();
bool WiFi_Connected();
void Save_Config_Callback();

void Programming_OTA();
// void isrInit();
// void resetESP(void *rEsp);
void blynkConnect();
// void blynkLed(void *rEsp);
void saveStatus();
void readStatus();

#endif