// Import required libraries
#include <WiFi.h>
//#include "ESPAsyncWebServer.h"
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiAP.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "Das ist kaputt";
const char* password = "Asdfghjkl";

// Create AsyncWebServer object on port 80
//AsyncWebServer server(80);
WebServer server(80);

void handleRoot() {
  String myFile = "/index.html";
  if (SPIFFS.exists(myFile)) {
    Serial.println(F("myFile founded on   SPIFFS"));   //ok
    File file = SPIFFS.open(myFile, "r");
    //server.send(200, "text/html", file);
    size_t sent = server.streamFile(file, "text/html" );
  }
  else
  {
    Serial.println(F("D49 stylsheet not found on SPIFFS"));
    handleNotFound;
  }
}
void handleCSS() {
  String myFile = "/style.css";
  if (SPIFFS.exists(myFile)) {
    Serial.println(F("myFile founded on   SPIFFS"));   //ok
    File file = SPIFFS.open(myFile, "r");
    //server.send(200, "text/css", file);
    size_t sent = server.streamFile(file, "text/css" );
    file.close();
  }
  else
  {
    Serial.println(F("D49 stylsheet not found on SPIFFS"));
    handleNotFound;
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.print(message);
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  //  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Create Wi-Fi access point
  //WiFi.softAP(ssid, password);
  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  // Print ESP32 Local IP Address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", handleRoot);

  // Route to load style.css file
  server.on("/style.css", handleCSS);

  server.onNotFound(handleNotFound);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}
