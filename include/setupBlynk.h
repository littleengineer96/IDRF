#ifndef SETUPBLYNK_H
#define SETUPBLYNK_H

#include "prototypes.h"

extern char blynk_token[35];
extern char blynk_server[17];
extern char blynk_port[5];

int BLYNK_connect();
int BLYNK_reconnect(unsigned int connection_attempts);
void BLYNK_connection(unsigned int connection_attempts);

// void setupBlynk()
// {
//   while ((blynk.ConnectionAttemptsCounter != BLYNK_CONNECTION_ATTEMPTS) && (blynk.ConectionState != BLYNK_CONNECTED_ST))
//   {
//     blynkConnect(); // conecta ao Blynk
//   }

//   if (blynk.ConectionState != BLYNK_CONNECTED_ST)
//   {
//     Serial.println("\nTentativa de conexão ao Blynk excedida.");
//     Serial.println("modo offline ativo");
//     // server.begin();
//     blynk.ConnectingVerify = 0; // flag zerada para ter uma contagem exata das tentativas posteriores
//     delay(DELAY);
//     bOffLineMode = true; // só entra no modo offline se conseguir se conectar a rede e não conseguir se conectar ao Blynk.
//   }
// }

void BLYNK_connection(unsigned int connection_attempts)
{
  while (connection_attempts)
  {
    if (BLYNK_connect())
    {
      // connection_attempts = 0;
      break;
    }
    else
    {
      connection_attempts -= 1;
    }
  }

  if (!Blynk.connect())
  {
    Serial.println("Device not connected to Blynk");
    Serial.println("The device will restart");
    ESP.restart();
  }
}

int BLYNK_reconnect(unsigned int connection_attempts)
{
  if (!Blynk.connect())
  {
    BLYNK_connection(connection_attempts);
  }
  else
  {
    return SUCCESS;
  }
}

int BLYNK_connect()
{

  Serial.println("\nConectando ao Blynk...");

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Device not connected WiFi");
    return ERRO;
  }

  // Blynk.config(blynk_token);
  //  Blynk.config("CmmH2Eq9mMfW-EclrCYV8uwR5Zn-eWwX", IPAddress(34, 95, 235, 246), 8080);
  // String pt = blynk_port;
  // uint16_t port = pt.toInt();

  Blynk.config(blynk_token, blynk_server, 8080);

  // if (Blynk.connect(TIME_OUT_BLYNK_CONNECT))
  if (Blynk.connect())
  {
    Serial.println("Connected to Blynk!");
    return SUCCESS;
  }
  else
  {
    Serial.println("Error Connect to Blynk.");
    return ERRO;
  }
}

#endif