#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266HTTPClient.h>

#include "ntc10k.h"
#include "ds18s20.h"
#include "http_lib.h"
#include "controller.h"

#define ANALOG_PIN A0
#define DS18S20_PIN D2
#define RELAY_PIN D1

OneWire dsBus(DS18S20_PIN);  // on digital pin 2
struct ControllerConfig conf;

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;

  pinMode(ANALOG_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  if(!wifiManager.autoConnect("GelaCerva")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  Serial.print("Connecting to ");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  // double temperature = getTemp(&dsBus);
  double temperature = thermistor2Temp(analogRead(ANALOG_PIN));

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
      send_data(temperature);
  }

  // Todo: Get config online
  conf.setpoint = 5.0;
  conf.band = 2.0;
  conf.max_range = 150.0;
  conf.min_range = -120.0;
  conf.relayPin = RELAY_PIN;
  conf.ledPin = BUILTIN_LED;

  controlerStateMachine(&conf, temperature);

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("State: ");
  Serial.println(state);

  delay(3000);
}
