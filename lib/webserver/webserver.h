#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

void initSPIFFS();
void setupWebServer();
void getRDSInfo(AsyncWebServerRequest *request);
String getLocationKey();
void getWeatherInfo(AsyncWebServerRequest *request);
void connectToInternetRadioStation(AsyncWebServerRequest *request);