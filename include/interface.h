#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <Arduino.h>
#include <WebServer.h>
#include "mrfc522_.h"
extern WiFiServer server;
extern WiFiClient client;
void handle_htmlStart();
void handle_homePage();

void handle_interface()
{
    // Variable to store the HTTP request
    String header;

    // Current time
    unsigned long currentTime = millis();
    // Previous time
    unsigned long previousTime = 0;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 1000;

    // WiFiClient client = server.available(); // Listen for incoming clients
    client = server.available(); // Listen for incoming clients

    // WiFiClient client = server.client();
    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("[interface] New Client"); // print a message out in the serial port
        String currentLine = "";                  // make a String to hold incoming data from the client

        while (client.connected() && ((currentTime - previousTime) <= timeoutTime))
        { // loop while the client's connected
            currentTime = millis();
            // Serial.println("New Client -> while.");
            // print a message out in the serial port

            if (client.available())
            {
                // print a message out in the serial port
                // if there's bytes to read from the client,
                // char c = client.read(); // read a byte, then
                // Serial.write(c);        // print it out the serial monitor
                // header += c;

                Serial.println(".");
                header = client.readString();
                // header = client.readStringUntil('HTTP');

                // getReceived = header.substring(header.indexOf("GET /"),header.indexOf("HTTP"));
                Serial.println(" === Client Received === \n" + header);

                // // if (c == '\n')

                /**/
                // if (!client.available())
                // { // if the byte is a newline character
                //     // if the current line is blank, you got two newline characters in a row.
                //     // that's the end of the client HTTP request, so send a response:

                //     Serial.println("..");
                //     if (currentLine.length() == 0)
                //     {
                /**/

                Serial.println("...");
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();

                // turns the GPIOs on and off

                if (header.indexOf("GET /send") >= 0)
                {
                }
                else if (header.indexOf("GET /savescript") >= 0)
                {
                    // myInterpreter.commandList(myScript.codeFeedback);
                }
                else if (header.indexOf("GET /openScript") >= 0)
                { // myScript.save();
                }
                else if (header.indexOf("GET /runScript") >= 0)
                {
                    // myScript.save();
                }
                else if (header.indexOf("GET /runscript1") >= 0)
                {
                    // myScript.save();
                }

                else if (header.indexOf("GET /comand?fname=") >= 0)
                {
                }

                if (header.indexOf("GET /ssidwifi") >= 0)
                {
                    // ssidAux = header.substring(header.indexOf("GET /ssidwifi") + 13, header.indexOf(" HTTP/"));
                }
                else if (header.indexOf("GET /confWifi") >= 0)
                {
                    // bRedeResult = true;
                    String ssid = header.substring(header.indexOf("?fssid=") + 7, header.indexOf("&fpass="));
                    String pass = header.substring(header.indexOf("&fpass=") + 7, header.indexOf(" HTTP/"));
                    Serial.println("SSID Receivend: " + ssid + " length:" + String(ssid.length()));
                    Serial.println("PASS Receivend: " + pass + " length:" + String(pass.length()));

                    char ssid_rede[ssid.length() + 1];
                    char pass_rede[pass.length() + 1];

                    ssid.toCharArray(ssid_rede, ssid.length() + 1);
                    pass.toCharArray(pass_rede, pass.length() + 1);
                    Serial.println("SSID 2 Receivend: " + String(ssid_rede));
                    Serial.println("PASS 2 Receivend: " + String(pass_rede));

                    // connect_saveParamWiFi(ssid_rede, pass_rede);
                    //  connect_wifi();

                    // WiFi.begin(ssid_rede, pass_rede);
                    // Serial.println("Connecting to WiFi..");
                    // uint8_t attempts = 0;
                    // while (attempts < 100)
                    // {
                    //     delay(500);

                    //     if (WiFi.status() != WL_CONNECTED)
                    //     {
                    //         attempts += 1;
                    //         WiFi.begin(ssid_rede, pass_rede);
                    //     }
                    //     else
                    //     {
                    //         break;
                    //     }
                    // }

                    if (WiFi.status() == WL_CONNECTED)
                    {
                        Serial.println("Connecting to WiFi..sucess!");
                    }
                    else
                    {
                        Serial.println("Connecting to WiFi..error!");
                    }

                    // auxRede = WiFi_State();
                    // char pass_rede [] = pass;
                }

                /* page select */
                if (header.indexOf("GET /home") >= 0)
                {
                    // menu.page = _PAGE_HOME;
                    // menu.botton1 = "\"btn2\"";        // home
                    // menu.botton2 = "\"btn success\""; // script
                    // menu.botton5 = menu.botton2;      // help
                    // menu.botton6 = menu.botton2;      // agendamento
                    // menu.botton7 = menu.botton2;      // control
                    // menu.botton8 = menu.botton2;      // wifi
                }
                else if (header.indexOf("GET /script") >= 0)
                {

                    // if (header.indexOf("GET /script/new") >= 0)
                    // {
                    //     menu.page = _PAGE_SCRIPT_NEW;
                    // }
                    // else if (header.indexOf("GET /script/open") >= 0)
                    // {
                    //     menu.page = _PAGE_SCRIPT_OPEN;
                    // }
                    // else if (header.indexOf("GET /script/run") >= 0)
                    // {
                    //     menu.page = _PAGE_SCRIPT_RUN;
                    // }
                    // else
                    // {
                    //     menu.page = _PAGE_SCRIPT;
                    // }

                    // menu.botton1 = "\"btn success\""; // home
                    // menu.botton2 = "\"btn2\"";        // script
                    // menu.botton5 = menu.botton1;      // help
                    // menu.botton6 = menu.botton1;      // agendamento
                    // menu.botton7 = menu.botton1;      // control
                    // menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /open") >= 0)
                {
                    // menu.page = _PAGE_SCRIPT_OPEN;
                    // menu.botton1 = "\"btn success\""; // home
                    // menu.botton2 = "\"btn2\"";        // script
                    // menu.botton5 = menu.botton1;      // help
                    // menu.botton6 = menu.botton1;      // agendamento
                    // menu.botton7 = menu.botton1;      // control
                    // menu.botton8 = menu.botton1;      // wifi
                    MRFC522_open(3000);

                }
                else if (header.indexOf("GET /agendamento") >= 0)
                {
                    // menu.page = _PAGE_AGEND_SCRIPT;
                    // menu.botton1 = "\"btn success\""; // home
                    // menu.botton2 = menu.botton1;      // script
                    // menu.botton5 = menu.botton1;      // help
                    // menu.botton6 = "\"btn2\"";        // agendamento
                    // menu.botton7 = menu.botton1;      // control
                    // menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /control") >= 0)
                {
                    // menu.page = _PAGE_CONTROL;
                    // menu.botton1 = "\"btn success\""; // home
                    // menu.botton2 = menu.botton1;      // script
                    // menu.botton5 = menu.botton1;      // help
                    // menu.botton6 = menu.botton1;      // agendamento
                    // menu.botton7 = "\"btn2\"";        // control
                    // menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /help") >= 0)
                {
                    // menu.page = _PAGE_HELP_COMAND;
                    // menu.botton1 = "\"btn success\""; // home
                    // menu.botton2 = menu.botton1;      // script
                    // menu.botton5 = "\"btn2\"";        // help
                    // menu.botton6 = menu.botton1;      // agendamento
                    // menu.botton7 = menu.botton1;      // control
                    // menu.botton8 = menu.botton1;      // wifi
                }
                else if (header.indexOf("GET /wifi") >= 0)
                {
                    // menu.page = _PAGE_WIFI;
                    // menu.botton1 = "\"btn success\""; // home
                    // menu.botton2 = menu.botton1;      // script
                    // menu.botton5 = menu.botton1;      // help
                    // menu.botton6 = menu.botton1;      // agendamento
                    // menu.botton7 = menu.botton1;      // control
                    // menu.botton8 = "\"btn2\"";        // wifi
                }

                // Web Page Heading
                handle_htmlStart();

                // if (menu.page == _PAGE_HOME)
                // {
                handle_homePage();
                // }

                // else if (menu.page == _PAGE_SCRIPT)
                // {
                //     handle_scriptPage();
                // }
                // else if (menu.page == _PAGE_SCRIPT_NEW)
                // {
                //     handle_scriptNew();
                // }
                // else if (menu.page == _PAGE_SCRIPT_OPEN)
                // {
                //     handle_scriptOpen();
                // }
                // else if (menu.page == _PAGE_SCRIPT_RUN)
                // {
                //     handle_scriptRun();
                // }
                // else if (menu.page == _PAGE_SCRIPT_RUN_1)
                // {
                //     handle_scriptRun1();
                // }

                // else if (menu.page == _PAGE_MANG_SCRIPT)
                // {
                //     handle_scriptManPage();
                // }

                // else if (menu.page == _PAGE_AGEND_SCRIPT)
                // {
                //     handle_agendScriptPage();
                // }

                // else if (menu.page == _PAGE_HELP_COMAND)
                // {
                //     handle_helpCommandPage();
                // }

                // else if (menu.page == _PAGE_CONTROL)
                // {
                //     handle_controlPage();
                // }
                // else if (menu.page == _PAGE_WIFI)
                // {
                //     handle_wifi();
                // }
                // else
                // {
                //     handle_notFoundPage();
                // }

                // }

                // Break out of the while loop
                break;

                /**/
                //     }
                //     else
                //     { // if you got a newline, then clear currentLine
                //         currentLine = "";
                //     }
                // }
                /**/

                // else if (c != '\r')
                // {                     // if you got anything else but a carriage return character,
                //     currentLine += c; // add it to the end of the currentLine
                // }
            }
        } // end while

        // Clear the header variable
        header = "";
        // Close the connection
        client.flush();
        client.stop();
        Serial.println("[interface] Client disconnected");
    } // end if
}

void handle_htmlStart()
{
    // Display the HTML web page
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");

    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences
    client.println("<style>");
    client.println("html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;border-radius: 24px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #555555;}");
    client.println("input[type=text] {width: 50%; padding: 12px 40px; margin: 2px 0; box-sizing: border-box; border: 2px solid #ccc; border-radius: 4px;}");
    client.println("input[type=submit] {  background-color: #04AA6D;border: none;color: white;padding: 14px 40px;text-decoration: none;margin: 4px 2px;cursor: pointer;border-radius: 4px;}");
    client.println("div.fixed {position: fixed; top: 1; left:1;} ");
    client.println(".btn {border: 2px solid black;border-radius: 5px;background-color: white;color: black;padding: 14px 40px;font-size: 16px;cursor: pointer;} .success {border-color: #04AA6D;color: green;} .success:hover {background-color: #04AA6D; color: white;}");
    client.println(".btn2 {border: 2px solid black; border-radius: 5px; color: black;padding: 14px 40px;font-size: 16px;cursor: pointer;border-color: #04AA6D;background-color: #04AA6D;color: white;}");
    client.println(".btn3 {border: 2px solid black; border-radius: 5px; color: black;padding: 14px 40px;font-size: 16px;cursor: default;border-color: #04AA6D;background-color: #04AA6D;color: white; opacity: 0.6;}");
    client.println("</style>");
    client.println("</head>");

    client.println("<body>");
    // client.println("<div class=\"fixed\" ><a href=\"/home\"><button class=" + menu.botton1 + "> &nbsp; &nbsp; home &nbsp; &nbsp;</button></a>");
    // client.println("<p><a href=\"/script\"><button class=" + menu.botton2 + "> &nbsp; &nbsp; script &nbsp; &nbsp; </button></a></p>");
    // client.println("<p><a href=\"/agendamento\"><button class=" + menu.botton6 + ">scheduling</button></a></p>");
    // client.println("<p><a href=\"/control\"><button class=" + menu.botton7 + "> &nbsp; controle &nbsp; </button></a></p>");
    // client.println("<p><a href=\"/wifi\"><button class=" + menu.botton8 + "> &nbsp; &nbsp; &nbsp; WiFi  &nbsp; &nbsp; </button></a></p>");
    // client.println("<p><a href=\"/help\"><button class=" + menu.botton5 + "> &nbsp; &nbsp; &nbsp; help  &nbsp; &nbsp; </button></a></p></div>");
}

void handle_homePage()
{
    Serial.println("handle_homePage");
    // String stateWiFi = WiFi.SSID();
    // WiFi.status();
    // client.println("<meta http-equiv=\"refresh\" content=\"10\">");
    // client.println("<h1> Robo-Cam Control </h1>");
    // client.println("<p> Data: " + ntp.getFormattedDate() + " </p>");
    // client.println("<p> Hora: " + ntp.getFormattedTime() + " </p>");
    // client.println("<h2> Status WiFi </h2>");
    // client.println("<p> Station Mode</p>");
    // client.println("<p> SSID: " + stateWiFi + "  (");
    // client.print(WiFi.RSSI());
    // client.print(")</p><p> IP: ");
    // client.print(WiFi.localIP());
    // client.println("</p>");
    // if (WiFi.status() == WL_CONNECTED)
    // {
    //     client.println("<p><a href=\"http://" + String(HOST_NAME) + ADDR_HOME + "\">" + "http://" + String(HOST_NAME) + ADDR_HOME + "</a></p>");
    // }
    // client.println("<p> Status: " + WiFi_State() + " </p>");
    client.println("<a href=\"/open\"><button class=\"btn success\"> &nbsp; &nbsp; Open &nbsp; &nbsp;</button></a>");

    // client.println("<p> AP Mode</p>");
    // client.println("<p> IP:");
    // client.print(WiFi.softAPIP());
    // client.print(" </p>");

    // client.println("<h2> Status Bateria </h2>");
    // String ligacao;
    // if (bateria.StateSerieOn)
    // {
    //     ligacao = "Serie";
    // }
    // else
    // {
    //     ligacao = "Paralelo";
    // }

    // if (bateria.StateCharging)
    // {
    //     client.println("<p> Bateria: Carregando...</p>");
    // }
    // client.println("<p> Ligacao dos Bancos:" + ligacao + "</p>");
    // // client.println("<p> Vin: " + String(bateria.ValueVin) + " V  </p>");
    // client.println("<p> 5V: " + String(bateria.ValueV5) + " V  </p>");
    // client.println("<p> Bank1: " + String(bateria.ValueVs1) + " V  " + String(bateria.ValuePercentVs1, 0) + "%</p>");
    // client.println("<p> Bank2: " + String(bateria.ValueVs2) + " V  " + String(bateria.ValuePercentVs2, 0) + "%</p>");
    // client.println("<p> Bank3: " + String(bateria.ValueVs3) + " V  " + String(bateria.ValuePercentVs3, 0) + "%</p>");
    // client.println("<p> Serie: " + String(bateria.ValueVtotal) + " V  " + String(bateria.ValuePercentTotal) + "%</p>");

    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line
    client.println();
}
#endif