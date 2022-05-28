#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include <globaldef.h>
#include <prototypes.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

// extern Verify wifi;
// extern Verify blynk;
// extern ESP_32 MyESP32;
// extern Verify blynk;
extern char blynk_token[34];
extern bool f_SerialOut_ON;

extern unsigned long TimeOutConnect;
extern unsigned long TimeOut;
// extern bool shouldSaveConfig;
extern const char esp32_rede[20];
extern const char esp32_pass[20];

bool shouldSaveConfig = false;
bool tokenInvalid = false;
enum Return
{
  NULL_ = -1,
  ERRO_ = 0,
  SUCCESS_ = 1,
  ZERO_ = 0,
  INVALID_TOKEN = 2
};

enum CurrentState
{
  CONNECT_WIFI_STT,
  CONNECT_BLYNK_STT,
  CONNECT_SUCESS
};

enum WiFi_Attempts
{
  WIFI_ASSIST_ATTEMPTS = 4,
  WIFI_ASSIST_TIMEOUT_MILLISEG = 90000,
  WIFI_CONNECT_ATTEMPTS = 4,
  WIFI_CONNECT_TIMEOUT_MILLISEG = 90000,
  WIFI_RECONNECT_ATTEMPTS = 4,
  WIFI_RECONNECT_TIMEOUT_MILLISEG = 10000,
  BLYNK_ASSIST_ATTEMPTS = 4,
  BLYNK_ASSIST_TIMEOUT_MILLISEG = 90000,
  BLYNK_CONNECT_ATTEMPTS = 4,
  BLYNK_CONNECT_TIMEOUT_MILLISEG = 30000,
  BLYNK_RECONNECT_ATTEMPTS = 4,
  BLYNK_RECONNECT_TIMEOUT_MILLISEG = 30000
};

enum WiFi_status
{
  WS_NO_SHIELD = 255, // for compatibility with WiFi Shield library
  WS_IDLE_STATUS = 0,
  WS_NO_SSID_AVAIL = 1,
  WS_SCAN_COMPLETED = 2,
  WS_CONNECTED = 3,
  WS_CONNECT_FAILED = 4,
  WS_CONNECTION_LOST = 5,
  WS_DISCONNECTED = 6,
  WS_RECONNECT = 7,
  WS_ERRO_TRYNG_TO_CONNECT = 8,
  WS_CONNECT_SUCCESS = 9,
  WS_CURRENT_STATE = 10
};

enum Blynk_status
{
  BS_DISCONNECT = 0,
  BS_CONNECTED = 1,
  BS_CONNECT_FAILED = 2,
  BS_NOT_CONNECTED_WIFI = 3,
  BS_INVALID_TOKEN = 4,
  BS_ERRO_TRYNG_TO_CONNECT = 8,
  BS_CONNECT_SUCCESS = 9,
  BS_CURRENT_STATE = 10
};

int Assistant_WiFi()
{
  int back = NULL_;
  int attempts = WIFI_ASSIST_ATTEMPTS;
  int timeOut = WIFI_ASSIST_TIMEOUT_MILLISEG;
  int state = WiFi.status();
  unsigned long time = millis();

  String step;
  String result;
  String process = " | CONNECT] WiFi: ";

#if DEBUG == 1
  Serial.println(process + "DEBUG");
#endif

  while (attempts && timeOut)
  {

#if DEBUG == 1
    Serial.println("\n[DEBUG] IN > \tstate: " + String(state) + " \tattempts: " + String(attempts) + " \ttime: " + String(millis() - time) +
                   "ms \ttimeOut: " + String(timeOut) + "ms \tback: " + String(back));
#endif

    switch (state)
    {
    case WS_CURRENT_STATE:
      step = "WS_CURRENT_STATE > ";
      step += WiFi_State();
      state = WiFi.status();
      break;
    case WS_NO_SHIELD:
      step = "WS_NO_SHIELD > ";
      state = WS_CONNECTED;
      step += "WS_CONNECTED";
      break;
    case WS_IDLE_STATUS:
      step = "WS_IDLE_STATUS > ";
      state = WS_RECONNECT;
      step += "WS_RECONNECT";
      break;
    case WS_NO_SSID_AVAIL:
      step = "WS_NO_SSID_AVAIL > ";
      state = WS_CONNECTED;
      step += "WS_CONNECTED";
      break;
    case WS_SCAN_COMPLETED:
      step = "WS_SCAN_COMPLETED > ";
      state = WS_CONNECTED;
      step += "WS_CONNECTED";
      break;
    case WS_CONNECTED:
      step = "WS_CONNECTED > ";
      back = WiFi_Manager();
      if (back == SUCCESS_)
      {
        step += "WS_CONNECT_SUCCESS";
        state = WS_CONNECT_SUCCESS;
      }
      else
      {
        step += "WS_CONNECT_FAILED";
        state = WS_CONNECT_FAILED;
        attempts -= 1;
      }
      break;
    case WS_CONNECT_FAILED:
      step = "WS_CONNECT_FAILED > ";
      state = WS_CONNECTED;
      step += "WS_CONNECTED";
      break;
    case WS_CONNECTION_LOST:
      step = "WS_CONNECTION_LOST > ";
      state = WS_RECONNECT;
      step += "WS_RECONNECT";
      break;
    case WS_DISCONNECTED:
      step = "WS_DISCONNECTED > ";
      state = WS_RECONNECT;
      step += "WS_RECONNECT";
      break;
    case WS_RECONNECT:
      step = "WS_RECONNECT > ";
      back = Reconnect_WiFi();
      if (back)
      {
        step += "WS_CONNECT_SUCCESS";
        state = WS_CONNECT_SUCCESS;
      }
      else
      {
        attempts -= 1;
        if (attempts > 1)
        {
          step += "WS_CONNECT_FAILED";
          state = WS_CONNECT_FAILED;
        }
        else
        {
          step += "WS_ERRO_TRYNG_TO_CONNECT";
          state = WS_ERRO_TRYNG_TO_CONNECT;
        }
      }
      break;
    case WS_ERRO_TRYNG_TO_CONNECT:
      step = "ERRO_TRYNG_TO_CONNECT";
      attempts = ZERO_;
      // return back;
      break;
    case WS_CONNECT_SUCCESS:
      step = "CONNECT_SUCCESS";
      attempts = ZERO_;
      // return back;
      break;
    default:
      step = "ERRO_STATE_MACHINE";
      attempts = ZERO_;
      // return "WL_ERRO";
      break;
    }
    Serial.println("[" + String(millis()) + process + step);

    if (millis() - time > timeOut)
    {
      timeOut = ZERO_;
      attempts = ZERO_;
      step = "CONNECTION TIMEOUT.";
      Serial.println("[" + String(millis()) + process + step);
    }

#if DEBUG == 1
    Serial.println("[DEBUG] OUT > \tstate: " + String(state) + " \tattempts: " + String(attempts) + " \ttime: " + String(millis() - time) +
                   "ms \ttimeOut: " + String(timeOut) + "ms \tback: " + String(back));
#endif
  }

#if DEBUG == 1
  Serial.println(process + "return: " + String(back));
#endif

  return back;
}

int Assistant_Blynk()
{
  int back = NULL_;
  int attempts = BLYNK_ASSIST_ATTEMPTS;
  int timeOut = BLYNK_ASSIST_TIMEOUT_MILLISEG;
  int state = Blynk.connected();
  unsigned long time = millis();

  String step;
  String result;
  String process = " | CONNECT] Blynk: ";

#if DEBUG == 1
  Serial.println(process + "DEBUG");
#endif

  while (attempts && timeOut)
  {
#if DEBUG == 1
    Serial.println("\n[DEBUG] IN > \tstate: " + String(state) + " \tattempts: " + String(attempts) + " \ttime: " + String(millis() - time) +
                   "ms \ttimeOut: " + String(timeOut) + "ms \tback: " + String(back));
#endif
    switch (state)
    {
    case BS_CURRENT_STATE:
      step = "BS_CURRENT_STATE > ";
      state = Blynk.connected();
      break;
    case BS_DISCONNECT:
      step = "BS_DISCONNECT > ";
      state = BS_CONNECTED;
      step += "BS_CONNECTED";
      break;
    case BS_CONNECT_FAILED:
      step = "BS_CONNECT_FAILED > ";
      state = BS_CONNECTED;
      step += "BS_CONNECTED";
      break;
    case BS_CONNECTED:
      step = "BS_CONNECTED > ";
      back = Blynk_Connect();
      if (back == SUCCESS_)
      {
        step += "BS_CONNECT_SUCCESS";
        state = BS_CONNECT_SUCCESS;
      }
      else if (back == ERRO_)
      {
        attempts -= 1;
        if (attempts > 1)
        {
          step += "BS_CONNECT_FAILED > ";
          state = BS_CONNECT_FAILED;
        }
        else
        {
          step += "BS_ERRO_TRYNG_TO_CONNECT";
          state = BS_ERRO_TRYNG_TO_CONNECT;
        }
      }
      else if (back == INVALID_TOKEN)
      {
        step += "BS_INVALID_TOKEN >";
        state = BS_INVALID_TOKEN;
      }
      else
      {
        step += "BS_NOT_CONNECTED_WIFI";
        state = BS_NOT_CONNECTED_WIFI;
      }
      break;
    case BS_NOT_CONNECTED_WIFI:
      step = "BS_NOT_CONNECTED_WIFI";
      attempts = ZERO_;
      break;
    case BS_INVALID_TOKEN:
      step = "BS_INVALID_TOKEN >";
      back = WiFi_Manager();
      if (back == SUCCESS_)
      {
        step += "BS_CONNECTED";
        state = BS_CONNECTED;
      }
      else
      {
        step += "BS_CONNECT_FAILED >";
        state = BS_CONNECT_FAILED;
        attempts -= 1;
      }
      break;
    case BS_CONNECT_SUCCESS:
      step = "BS_CONNECT_SUCCESS";
      attempts = ZERO_;
      break;
    case BS_ERRO_TRYNG_TO_CONNECT:
      step = "BS_ERRO_TRYNG_TO_CONNECT";
      attempts = ZERO_;
      break;
    default:
      step = "ERRO_STATE_MACHINE";
      attempts = ZERO_;
      break;
    }

    Serial.println("[" + String(millis()) + process + step);

    if (millis() - time > timeOut)
    {
      timeOut = ZERO_;
      attempts = ZERO_;
      step = "CONNECTION TIMEOUT.";
      Serial.println("[" + String(millis()) + process + step);
    }

#if DEBUG == 1
    Serial.println("[DEBUG] OUT > \tstate: " + String(state) + " \tattempts: " + String(attempts) + " \ttime: " + String(millis() - time) +
                   "ms \ttimeOut: " + String(timeOut) + "ms \tback: " + String(back));
#endif
  }

#if DEBUG == 1
  Serial.println(process + "return: " + String(back));
#endif

  return back;
}

int Reconnect_WiFi()
{
  int back = NULL_;
  // int attempts = WIFI_RECONNECT_ATTEMPTS;
  int timeOut = WIFI_RECONNECT_TIMEOUT_MILLISEG;
  unsigned long time;

  if (WiFi_Connected())
  {
    back = SUCESS;
    return back;
  }

  if (WiFi.status() == WL_DISCONNECTED)
  {
    back = WiFi.reconnect();
  }

  if (WiFi.status() == WL_CONNECTION_LOST)
  {
    back = WiFi.reconnect();
  }

  if (WiFi.status() == WL_CONNECT_FAILED)
  {
    back = WiFi.reconnect();
  }

  if (WiFi.status() == WL_NO_SHIELD)
  {
    back = WiFi.reconnect();
  }

  if (WiFi.status() == WL_IDLE_STATUS)
  {
    time = millis();
    while ((WiFi.status() == WL_IDLE_STATUS) && timeOut)
    {
      timeOut--;
      if ((millis() - time) > timeOut)
      {
        timeOut = ZERO_;
      }
    }

    back = WiFi_Connected();
  }

  time = millis();
  timeOut = WIFI_RECONNECT_ATTEMPTS;

  while ((WiFi.status() == WL_IDLE_STATUS) && timeOut)
  {
    timeOut--;
    if ((millis() - time) > timeOut)
    {
      timeOut = ZERO_;
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    back = SUCESS;
  }

  return back;
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

int WiFi_Manager()
{
  int back = NULL_;

  String step;
  String result;
  String process = " | WiFi_Manager] ";

  if ((WiFi.status() != WL_CONNECTED) || tokenInvalid)
  {

    // if (!tokenInvalid)
    // {
    //   WiFi.disconnect();
    // }
    // wifi.ConectionState = CONNECTING_ST; // indica o status do dispositivo "tento se conectar"
    // wifi.ConnectingVerify++;             // conta a quantidade de vezes que o dispositivo tentou se conectar

    // Serial.println("mounting FS...");

    if (SPIFFS.begin())
    {
      // Serial.println("mounted file system");
      if (SPIFFS.exists("/config.json"))
      {
        // file exists, reading and loading
        // se existe o arquivo, carregue e leia
        // Serial.println("reading config file");
        File configFile = SPIFFS.open("/config.json", "r");
        if (configFile)
        {
          // Serial.println("opened config file");
          size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
          std::unique_ptr<char[]> buf(new char[size]);

          configFile.readBytes(buf.get(), size);
          DynamicJsonBuffer jsonBuffer;
          JsonObject &json = jsonBuffer.parseObject(buf.get());
          json.printTo(Serial);
          if (json.success())
          {
            // Serial.println("\nparsed json");
            strcpy(blynk_token, json["blynk_token"]);
          }
          else
          {
            // Serial.println("failed to load json config");
            step = "Failed to load json config";
            Serial.println("[" + String(millis()) + process + step);
          }
          configFile.close();
        }
      }
    }
    else
    {
      // Serial.println("failed to mount FS");
      step = "Failed to mount FS";
      Serial.println("[" + String(millis()) + process + step);
    } // end read
    //  digitalWrite(LED_, false);
    //   Serial.println(blynk_token);
    WiFiManagerParameter custom_blynk_token("blynk", "Blynk Token", blynk_token, SIZE_TOKEN);

    WiFiManager wifiManager;

    // wifiManager.resetSettings();
    wifiManager.setTimeout(TIMEOUT_PORTAL);
    wifiManager.setSaveConfigCallback(Save_Config_Callback);
    wifiManager.addParameter(&custom_blynk_token);
    wifiManager.setHostname("mydevice.com");
    // wifiManager.startConfigPortal(ssidAP, "12345678");
    // wifiManager.startWebPortal();
    // startConfigPortal

    if (tokenInvalid)
    {
      // if (tokenInvalid)
      // {
      //   Serial.println("--------");
      //   wifiManager.startConfigPortal(ssidAP,passAP);
      //   /* code */
      // }
    }
    else
    {
      if (!wifiManager.autoConnect(ssidAP, passAP))
      {
        // Serial.println("failed to connect and hit timeout");
        step = "Failed to connect and hit timeout";
        Serial.println("[" + String(millis()) + process + step);
      }
    }

    // Serial.println("connected...yeey :)");
    strcpy(blynk_token, custom_blynk_token.getValue());
    String leng = blynk_token;

    if (leng.length() != SIZE_TOKEN) // só reseta as configurações se conseguir se conectar a rede e a quantidade de caracteres do blynk_tokne for != SIZE_TOKEN
    {
      // Serial.println("Blynk_token está incorreto!");
      // Serial.println("O dispositivo será resetado..");

      step = "Invalid blynk token different size";
      Serial.println("[" + String(millis()) + process + step);
      char blynk_token_aux[34] = "YOUR_BLYNK_TOKEN_INVALID";

      for (int i = 0; i < 34; i++)
      {
        blynk_token[i] = blynk_token_aux[i];
      }
      Serial.println("TOKEN: " + String(blynk_token));
    }

    // save the custom parameters to FS
    // if (shouldSaveConfig && (wifi.ConectionState != NOT_CONNECTED_ST))
    if (shouldSaveConfig)
    {
      Serial.println("saving config");
      DynamicJsonBuffer jsonBuffer;
      JsonObject &json = jsonBuffer.createObject();
      json["blynk_token"] = blynk_token;

      File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile)
      {
        Serial.println("failed to open config file for writing");
        // rt = ERRO;
      }

      json.printTo(Serial);
      json.printTo(configFile);
      configFile.close();
      Serial.println("");
      // end save
    }

    // Serial.println("local ip:" + WiFi.localIP());
    // Serial.println(WiFi.localIP());
    String psk = WiFi.psk();
    String ssid = WiFi.SSID();

    Serial.println("SSID: " + ssid + " pass: " + psk);
    // Serial.println("Token: " + leng);
    //   Serial.println(leng.length());

    // if (((leng.length()) != SIZE_TOKEN) & (wifi.ConectionState != NOT_CONNECTED_ST)) // só reseta as configurações se conseguir se conectar a rede e a quantidade de caracteres do blynk_tokne for != SIZE_TOKEN
    // if (leng.length() != SIZE_TOKEN) // só reseta as configurações se conseguir se conectar a rede e a quantidade de caracteres do blynk_tokne for != SIZE_TOKEN

    // {
    //   Serial.println("Blynk_token está incorreto!");
    //   Serial.println("O dispositivo será resetado..");
    //   wifiManager.resetSettings();
    //   SPIFFS.format();
    //   ESP.restart();
    // }

    if (WiFi.status() == WL_CONNECTED) // caso aconteça a conexão
    {
      // wifi.ConectionState = CONNECTED_ST;
      // wifi.ConnectedVerify++;
      back = SUCCESS_;
    }
    else
    {
      back = ERRO_;
    }
  }
  else
  {
    back = SUCCESS_;
  }

  return back;
}

void Save_Config_Callback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

bool WiFi_Connected()
{
  int back = NULL_;

  if (WiFi.status() == WL_CONNECTED)
  {
    back = SUCCESS_;
  }
  else
  {
    back = ERRO_;
  }

  return back;
}

int Blynk_Connect()
{
  int back = NULL_;

  // if (WiFi.status() != WL_CONNECTED)
  // {
  //   return back;
  // }

  if (!WiFi_Connected())
  {
    return back;
  }

  back = Blynk.connected();
  if (back)
  {
    return back;
  }
  // blynk_token[2] = 'A';

  Blynk.config(blynk_token);

  back = Blynk.connect();

  if (Blynk.isTokenInvalid())
  {
    back = INVALID_TOKEN;
    tokenInvalid = true;
  }
  else
  {
    tokenInvalid = false;
  }

  //  Blynk.config("CmmH2Eq9mMfW-EclrCYV8uwR5Zn-eWwX", IPAddress(34, 95, 235, 246), 8080);

  // if (Blynk.connect(TIME_OUT_BLYNK_CONNECT))
  // {
  //   // Serial.println("\nConnected to Blynk!");
  //   blynk.ConectionState = BLYNK_CONNECTED_ST;
  //   blynk.ConnectedVerify++;
  //   back = SUCESS;
  // }
  // else
  // {
  //   // Serial.println("\nError Connect to Blynk.");
  //   blynk.ConectionState = BLYNK_NOT_CONNECTED_ST;
  //   blynk.NotConnectedVerify++;
  // }

  //  rtc.begin();
  //  timer.setTimeout(TIME_LED_OFF, blynkLed); //ativa a interrupção de 1 em 1s
  //  timer.setInterval(10000L, myTimerEvent); //ativa a interrupção de 1 em 1s

  //  resetHDW = false;                        //flag para reinicar o dispositivo
  //  stsLED = true;
  //  Serial.end();                            //desativa serial comunication

  //  pinMode(LED_, OUTPUT);
  //  digitalWrite(LED_, true);
  //  Blynk.email("engenheiro.automation@gmail.com", "Wiswitch-Start!", "Dispositivo iniciado, acordado.");

  // blynk.ConnectionAttemptsCounter++;
  return back;
}

#endif