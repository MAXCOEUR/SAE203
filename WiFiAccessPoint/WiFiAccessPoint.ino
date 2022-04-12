/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/


#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ChainableLED.h>


#define A0 26
#define A1 25
#define A2 34
#define A3 39
#define A4 36
#define A5 4

#define D2 14
#define D3 32
#define D4 15
#define D5 14

#define RX RX
#define TX TX

#define SCL 22
#define SDA 23
#define BB 32
#define BC 14

ChainableLED leds(D2,D3,1); //LED

// Set these to your desired credentials.
const char *ssid = "ESP32";
const char *password = "Max2003?";

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
bool page1=true;
bool page2=false;

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

void setup() {
  leds.init();
  Serial.begin(115200);
  // Initialize the output variables as outputs

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

String Page1="<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p><a href=\"/page2\"><button class=\"button\">page2</button></a></p>\
  </body>\
</html>";

String Page2="<!DOCTYPE html><html><body><h1>ESP32 Web Server</h1>\
  <p><a href=\"/page1\"><button class=\"button\">page1</button></a></p>\
</body>";

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /page2") >= 0) {
              leds.setColorRGB(0, 255, 0, 0);
              page1=false;
              page2=true;
            }
            if (header.indexOf("GET /page1") >= 0) {
              leds.setColorRGB(0, 255, 0, 0);
              page1=true;
              page2=false;
            }
            
            // Display the HTML web page      
            if (page1) {
              client.println(Page1);
            }
            // If the output27State is off, it displays the ON button       
            if (page2) {
              client.println(Page2);
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
