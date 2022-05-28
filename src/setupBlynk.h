#ifndef SETUPBLYNK_H
#define SETUPBLYNK_H

#include <Arduino.h>
#include <globaldef.h>

extern bool bOffLineMode;
extern Verify blynk;
extern char blynk_token[34];

void blynkConnect();

void setupBlynk()
{
  while ((blynk.ConnectionAttemptsCounter != BLYNK_CONNECTION_ATTEMPTS) && (blynk.ConectionState != BLYNK_CONNECTED_ST))
  {
    blynkConnect(); // conecta ao Blynk
  }

  if (blynk.ConectionState != BLYNK_CONNECTED_ST)
  {
    Serial.println("\nTentativa de conexão ao Blynk excedida.");
    Serial.println("modo offline ativo");
    // server.begin();
    blynk.ConnectingVerify = 0; // flag zerada para ter uma contagem exata das tentativas posteriores
    delay(DELAY);
    bOffLineMode = true; // só entra no modo offline se conseguir se conectar a rede e não conseguir se conectar ao Blynk.
  }
}

void blynkConnect()
{

  Serial.println("\nConectando ao Blynk...");

  blynk.ConectionState = BLYNK_CONNECTING_ST;
  blynk.ConnectingVerify++;
  blynk.ConnectionAttemptsCounter++;

  Blynk.config(blynk_token);
  //  Blynk.config("CmmH2Eq9mMfW-EclrCYV8uwR5Zn-eWwX", IPAddress(34, 95, 235, 246), 8080);
  if (Blynk.connect(TIME_OUT_BLYNK_CONNECT))
  {
    Serial.println("\nConnected to Blynk!");
    blynk.ConectionState = BLYNK_CONNECTED_ST;
    blynk.ConnectedVerify++;
  }
  else
  {
    Serial.println("\nError Connect to Blynk.");
    blynk.ConectionState = BLYNK_NOT_CONNECTED_ST;
    blynk.NotConnectedVerify++;
  }

  Serial.println("\n----------------------");
  Serial.println("BLYNK_NOT_CONNECTED_ST:" + String(blynk.NotConnectedVerify));
  Serial.println("BLYNK_CONNECTING_ST:" + String(blynk.ConnectingVerify));
  Serial.println("BLYNK_CONNECTED_ST:" + String(blynk.ConnectedVerify));
  Serial.println("BLYNK_DISCONNECTED_ST:" + String(blynk.DisconnectedVerify));
  Serial.println("----------------------");

  blynk.ConnectionAttemptsCounter++;
}
#endif