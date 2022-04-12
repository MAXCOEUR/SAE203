/*
  ESP32 Web Server - AP Mode
  modified on 25 MAy 2019
  by Mohammadreza Akbari @ Electropeak
  Home
*/

#include <WiFi.h>
#include <WebServer.h>

// SSID & Password
const char* ssid = "ESP32";  // Enter your SSID here
const char* password = "Max2003?";  //Enter your Password here

// IP Address details
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


void setup() {
  Serial.begin(115200);

  // Create SoftAP
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);


  Serial.print("Connect to My access point: ");
  Serial.println(ssid);

  server.on("/", handle_root);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  server.handleClient();
}

// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>My First Web Server with ESP32 - AP Mode &#128522;</h1>\
<a href=\"page2.html\">page 2</a>\
</body>\
</html>";

// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML);
}
