/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)

  All_Free_Fonts_Demo
  Example for TFT_eSPI library
  Created by Bodmer 31/12/16

  WiFiSwitch Mashup 
  ShotokuTech 2 August 2020
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "Free_Fonts.h"
#include "SPI.h"
#include "TFT_eSPI.h"

//display Use hardware SPI
TFT_eSPI tft = TFT_eSPI();
unsigned long drawTime = 0;

// Set these to your desired credentials.
const char *ssid = "TTGOWAP";
const char *password = "11111111";

WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

    //display
    tft.begin();
    tft.setRotation(1);
    String payload = "WAITING";          
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(FF24);
    tft.drawString(payload, 1, 43, GFXFF);    
    
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<br><br>");            
            client.print("Click <a href=\"/1\">here</a> to RUN.<br>");

            client.print("<br><br>");
            client.print("Click <a href=\"/2\">here</a> to STOP.<br>");

            client.print("<br><br>");
            client.print("Click <a href=\"/3\">here</a> to RESET.<br>");            

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /R":
        if (currentLine.endsWith("GET /1")) {
                String payload = "RUNNING";          
                tft.fillScreen(TFT_BLACK);
                tft.setFreeFont(FF24);
                tft.drawString(payload, 1, 43, GFXFF);
          
        }
       
      if (currentLine.endsWith("GET /2")) {
                String payload = "STOPPED";          
                tft.fillScreen(TFT_BLACK);
                tft.setFreeFont(FF24);
                tft.drawString(payload, 1, 43, GFXFF);
          
        }
     
        if (currentLine.endsWith("GET /3")) {
                String payload = "RESETTING";
                client.println();
                tft.fillScreen(TFT_BLACK);
                tft.setFreeFont(FF23);
                tft.drawString(payload, 1, 43, GFXFF);
                delay(1000);
                ESP.restart();                
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
