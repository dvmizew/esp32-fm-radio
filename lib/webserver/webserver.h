#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

void setupWebServer();
void getRDSInfo(AsyncWebServerRequest *request);
void getWeatherInfo(AsyncWebServerRequest *request);
void connectToInternetRadioStation(AsyncWebServerRequest *request);