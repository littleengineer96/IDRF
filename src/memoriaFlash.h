#ifndef MEMORIAFLASH_H
#define MEMORIAFLASH_H

#include <Arduino.h>
#include <globaldef.h>

extern bool bBlynkButtonState;
extern bool bKeyPulseState;
extern bool bReleState;
extern String output2State;

void readStatus()
{
  if (!SPIFFS.begin())
  {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }
  else
  {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }

  if (SPIFFS.exists(DATA_STATE_ADDRESS))
  {
    Serial.println("reading config file");
    File configFile = SPIFFS.open(DATA_STATE_ADDRESS, "r");
    if (!configFile)
    {
      Serial.println("Erro ao abrir arquivo!");
    }
    else
    {
      String content = configFile.readStringUntil('\r'); // desconsidera '\r\n'
      Serial.print("leitura de estado: ");
      Serial.println(content);

      //      int tamanho = content.length();

      //      if (((content.compareTo("on")) == 0) || (tamanho == 3))
      if (content.compareTo("on") == 0)
      {
        Serial.println("Ligar Rele");
        digitalWrite(RELE_PIN, false); // liga o rele
        // Blynk.virtualWrite(ON_OFF_V2, 1); // altero o status do butão para on
        bBlynkButtonState = true;
        bReleState = true;
        output2State = "on";
      }
    }

    configFile.close();
  }
  else
  {
    Serial.println("arquivo não existe");
    Serial.println("write config file");
    File configFile = SPIFFS.open(DATA_STATE_ADDRESS, "w+");
    if (!configFile)
    {
      Serial.println("Erro ao abrir arquivo!");
    }
    else
    {
      configFile.println("off");
      Serial.print("arquivo criado gravado: ");
      Serial.println("off");
    }
    configFile.close();
  }
}
#endif