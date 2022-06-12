// Import required libraries
#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Preferences.h>

using namespace httpsserver;

// Replace with your network credentials
const char* ssid = "Das ist kaputt";
const char* password = "Asdfghjkl";

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ResourceNode * nodeRoot = new ResourceNode("/", "GET", [](HTTPRequest * req, HTTPResponse * res) {
    res->println("Secure Hello World!!!");
    ResourceParameters * params = req->getParams();
    std::string paramName ="to_serial";
    std::string paramVal;
    if (params->getQueryParameter(paramName, paramVal)) {
      Serial.println();
      Serial.println();
      //Serial.print(paramName.c_str());
      Serial.print(paramVal.c_str());
      Serial.println();
      Serial.println();
    }
  });
  secureServer->registerNode(nodeRoot);

  ResourceNode * corsNode = new ResourceNode("/*", "OPTIONS", &corsCallback);
  secureServer->registerNode(corsNode);

  // Start server
  Serial.println("Starting HTTPS server...");
  secureServer->start();

  if (secureServer->isRunning()) {
    Serial.println("Server ready.");
  }
}

void loop() {
  secureServer->loop();
  delay(1);
}

/* ref
 * https://github.com/fhessel/esp32_https_server/issues/55#issuecomment-549375480
 * https://github.com/isemann/MeshCom_1.49/blob/c54ae1c1ebd8334e689e35d6d4015928d96edad1/src/mesh/http/WebServer.cpp
 * https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
 * https://arduino.stackexchange.com/questions/56365/arduino-convert-stdstring-to-string
 * https://github.com/fhessel/esp32_https_server
 */
