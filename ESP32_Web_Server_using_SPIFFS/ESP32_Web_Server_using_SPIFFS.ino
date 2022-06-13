// Import required libraries
#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Preferences.h>
#include <ESPmDNS.h>

using namespace httpsserver;

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

const char* nDNShostName = "esp32";

SSLCert * cert;
HTTPSServer * secureServer;
Preferences preferences;

void corsCallback(HTTPRequest * req, HTTPResponse * res) {
  Serial.println(">> CORS here...");
  res->setHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  res->setHeader("Access-Control-Allow-Origin",  "*");
  res->setHeader("Access-Control-Allow-Headers", "*");
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.print("Boot success \n\n");

  preferences.begin("certi_data", false);

  size_t pkLen = preferences.getBytesLength("PKData");
  size_t certLen = preferences.getBytesLength("CertData");

  if (pkLen && certLen) {

    uint8_t *pkBuffer = new uint8_t[pkLen];
    preferences.getBytes("PKData", pkBuffer, pkLen);

    uint8_t *certBuffer = new uint8_t[certLen];
    preferences.getBytes("CertData", certBuffer, certLen);

    cert = new SSLCert(certBuffer, certLen, pkBuffer, pkLen);
    Serial.println("Certificate loaded with success");
  }
  else {
    cert = new SSLCert();
    int createCertResult = createSelfSignedCert(
                             *cert,
                             KEYSIZE_1024,
                             "CN=tuteja.sameer,O=sam.tj@github.io,C=DE");

    if (createCertResult != 0) {
      Serial.printf("Error generating certificate");
      return;
    }
    preferences.putBytes("PKData", (uint8_t *)cert->getPKData(), cert->getPKLength());
    preferences.putBytes("CertData", (uint8_t *)cert->getCertData(), cert->getCertLength());
    Serial.println("Certificate created with success");
  }

  secureServer = new HTTPSServer(cert);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print ESP32 Local IP Address
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  if (!MDNS.begin(nDNShostName)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  ResourceNode * nodeRoot = new ResourceNode("/", "GET", [](HTTPRequest * req, HTTPResponse * res) {
    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><title>I am </title></head>");
    res->println("<body>");
    res->println("Use <a href=\"https://sam-tj.github.io/Web_Serial/\">this</a> to send data to serial.");
    res->println("</body>");
    res->println("</html>");

    ResourceParameters * params = req->getParams();
    std::string paramName = "to_serial";
    std::string paramVal;
    if (params->getQueryParameter(paramName, paramVal)) {
      Serial.println();
      Serial.println();
      Serial.print("Data received: ");
      //Serial.print(paramName.c_str());
      Serial.println(paramVal.c_str());
      Serial.println();
      Serial.println();
    }
  });
  secureServer->registerNode(nodeRoot);

  ResourceNode * corsNode = new ResourceNode("/*", "OPTIONS", &corsCallback);
  secureServer->registerNode(corsNode);

  ResourceNode * whoami = new ResourceNode("/whoami", "GET", [](HTTPRequest * req, HTTPResponse * res) {
    res->setHeader("Content-Type", "text/html");

    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><title>I am ");
    res->println(nDNShostName);
    res->println("</title></head>");
    res->println("<body>");
    res->println("<h3>Hi, here you can find more about me.</h3>");
    res->printf("<li>mDNS Name: %s.local", nDNShostName);
    res->printf("<li>IP Address: %s", WiFi.localIP().toString().c_str() );
    res->print("<br\><br\><p>Your server is running for ");
    res->print((int)(millis() / 1000), DEC);
    res->println(" seconds.</p>");
    res->println("</body>");
    res->println("</html>");
  });
  secureServer->registerNode(whoami);
  // Start server
  Serial.println("Starting HTTPS server...");
  secureServer->start();

  if (secureServer->isRunning()) {
    Serial.println("Server ready.");
  }

  MDNS.addService("https", "tcp", 443);
}

void loop() {
  secureServer->loop();
  delay(1);
}

/* ref
   https://github.com/fhessel/esp32_https_server/issues/55#issuecomment-549375480
   https://github.com/isemann/MeshCom_1.49/blob/c54ae1c1ebd8334e689e35d6d4015928d96edad1/src/mesh/http/WebServer.cpp
   https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
   https://arduino.stackexchange.com/questions/56365/arduino-convert-stdstring-to-string
   https://github.com/fhessel/esp32_https_server
*/
