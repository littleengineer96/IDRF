#ifndef _INTERFACE2_H_
#define _INTERFACE2_H_

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-relay-module-ac-web-server/

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO false

// Set number of relays
#define NUM_RELAYS 1

// Assign each GPIO to a relay
// int relayGPIOs[NUM_RELAYS] = { 2, 26, 27, 25, 33 };
int relayGPIOs[NUM_RELAYS] = {17};

// Replace with your network credentials
// const char* ssid = "TesteReley";
// const char* password = "12345678";
const char *ssid = "GAMEPLAY";
const char *password = "12345678";

const char *PARAM_INPUT_1 = "relay";
const char *PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
extern AsyncWebServer server;

String relayState(int numRelay);
String processor(const String &var);
void interface2Begin();
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %STATEWIFI%
  %STATEPORTA%
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}</script>
</body>
</html>
)rawliteral";


// Replaces placeholder with button section in your web page
String processor(const String &var)
{
    // Serial.println(var);
    if (var == "BUTTONPLACEHOLDER")
    {
        String pinState = "";
        if (digitalRead(PIN_REEDSWITCH) == 0)
        {
            pinState = "Aberta";
        }
        else
        {
            pinState = "Fechada";
        }
        String buttons = "";
        for (int i = 1; i <= NUM_RELAYS; i++)
        {
            String relayStateValue = relayState(i);
            buttons += "<h4>Open/Close</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + "><span class=\"slider\"></span></label>";
        }
        return buttons;
    }
    else if (var == "STATEPORTA")
    {
        String pinState = "";
        if (digitalRead(PIN_REEDSWITCH) == 0)
        {
            pinState = "Fechada";
        }
        else
        {
            pinState = "Aberta";
        }

        String state = "";
        state = "<h3>Porta: " + pinState + "</h3>";

        return state;
    }else if(var == "STATEWIFI")
    {
        const char myip = WiFi.localIP();
        String blynkState = "not Connected";
        if (Blynk.connected())
        {
            blynkState = "Connected";
        }
        
        String wifiState ="";
        wifiState += "<h4>SSID:"+WiFi.SSID()+"</h4>";
        wifiState += "<h4>Blynk:"+blynkState+"</h4>";

        return wifiState;

    }

    return String();
}

String relayState(int numRelay)
{
    if (RELAY_NO)
    {
        if (digitalRead(relayGPIOs[numRelay - 1]))
        {
            return "";
        }
        else
        {
            return "checked";
        }
    }
    else
    {
        if (digitalRead(relayGPIOs[numRelay - 1]))
        {
            return "checked";
        }
        else
        {
            return "";
        }
    }
    return "";
}

void interface2Begin()
{
    // Serial port for debugging purposes
    //   Serial.begin(115200);

    // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
    for (int i = 1; i <= NUM_RELAYS; i++)
    {
        pinMode(relayGPIOs[i - 1], OUTPUT);
        if (RELAY_NO)
        {
            digitalWrite(relayGPIOs[i - 1], HIGH);
            // MRFC522_open(3000);
        }
        else
        {
            digitalWrite(relayGPIOs[i - 1], LOW);
            // MRFC522_open(3000);
        }
    }

    // Connect to Wi-Fi
    //   WiFi.begin(ssid, password);
    //   while (WiFi.status() != WL_CONNECTED) {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi..");
    //   }

    // if (!MDNS.begin("mydevice"))
    if (!MDNS.begin(DEVICE_NAME))
    {
        Serial.print("\nerro ao iniciar mDNS, tentando novamente...");
    }
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });

    // Send a GET request to <ESP_IP>/update?relay=<inputMessage>&state=<inputMessage2>
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if (RELAY_NO) {
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt() - 1], !inputMessage2.toInt());
      } else {
        Serial.print("NC ");
        // digitalWrite(relayGPIOs[inputMessage.toInt() - 1], inputMessage2.toInt());
        MRFC522_open(3000);

      }
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    // String resp = "<meta http-equiv=\"refresh\" content=\"10\">";
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK"); });
    // request->send_P(200, "text/html", index_html, processor);});
    // server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
    //           {
    // String resp ="";

    // resp +="<p> Escaneando... </p>";
    // int countWifi = WiFi.scanNetworks();
    // if (countWifi > 0)
    // {
    //     if (countWifi)
    //     {
    //         resp +="<p> Redes WiFi encontradas: </p>";
    //     }
    //     else
    //     {
    //         resp += "<p> Rede WiFi encontrada: </p>";
    //     }

    //     for (size_t i = 0; i < countWifi; i++)
    //     {
    //         resp  +="<p> " + String(i + 1) + ":";
    //         // ssidAux = WiFi.SSID(i);
    //        resp  += "<a href=\"/ssidwifi" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</a>";
    //         resp  += " (";
    //         resp  += WiFi.RSSI(i);
    //         resp  += ")</p>";
    //     }
    // }
    // request->send(200, "text/plain", resp); });

    // Start server
    server.begin();
}

#endif