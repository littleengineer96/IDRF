#ifndef SAVESTATUS_H
#define SAVESTATUS_H

#include "prototypes.h"

// extern bool bReleState;

void saveStatus()
{
  if (SPIFFS.exists(DATA_STATE_ADDRESS))
  {
    Serial.println("write config file");
    File configFile = SPIFFS.open(DATA_STATE_ADDRESS, "w+");
    if (!configFile)
    {
      Serial.println("Erro ao abrir arquivo!");
    }
    else
    {
      if (bReleState)
      {
        configFile.println("on");
        Serial.print("gravou estado: ");
        Serial.println("on");
      }
      else
      {
        configFile.println("off");
        Serial.print("gravou estado: ");
        Serial.println("off");
      }
    }

    configFile.close();
  }
  else
  {
    Serial.println("arquivo não existe");
  }
}
#endif