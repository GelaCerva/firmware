#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <OneWire.h>              //DSB18B20 Temperature Sensor
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266HTTPClient.h>

bool send_temp(double t){
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://192.168.1.8:8000/api/temp/");  //Specify request destination
    http.addHeader("Content-Type", "application/json");
    String json_post = "{\"temperature\":" +  String(t) + ",\"device\":1}";
    int httpCode = http.POST(json_post); //Send the request

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);
    }

    http.end();   //Close connection
    return true;
}
