#include "webserver.h"

AsyncWebServer server(80);

void initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS, formatting...");
        SPIFFS.format();
        if (!SPIFFS.begin(true)) {
            Serial.println("Failed to initialize SPIFFS again");
            return;
        }
    }

    // Print SPIFFS information
    Serial.print("SPIFFS Total space: ");
    Serial.println(SPIFFS.totalBytes());
    Serial.print("SPIFFS Used space: ");
    Serial.println(SPIFFS.usedBytes());

}

void setupWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        File file = SPIFFS.open("/index.html", "r");
        if (!file) {
            request->send(404, "text/plain", "File not found");
            return;
        }

        String content = file.readString();
        request->send(200, "text/html", content);
        file.close();
    });

    server.on("/rds", HTTP_GET, [](AsyncWebServerRequest *request) {
        getRDSInfo(request);
    });

    server.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request) {
        getWeatherInfo(request);
    });

    server.on("/connectIRS", HTTP_GET, [](AsyncWebServerRequest *request) {
        connectToInternetRadioStation(request);
    });

    server.begin();
}

void getRDSInfo(AsyncWebServerRequest *request) {
    // get RDS info from some API
    String rdsInfo = "RDS Info: Radio Station Name";
    request->send(200, "text/plain", rdsInfo);
}

const char* apiKey = "WEATHER_API_KEY"; // your accuweather API key
const char* cityName = "YOUR_CITY_NAME"; // the city name for which you want to get the weather
const char* locationServerName = "http://dataservice.accuweather.com/locations/v1/cities/search";
const char* weatherServerName = "http://dataservice.accuweather.com/currentconditions/v1/";

String getLocationKey() {
    HTTPClient http;
    String serverPath = String(locationServerName) + "?apikey=" + apiKey + "&q=" + cityName;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        String locationKey = doc[0]["Key"].as<String>();
        http.end();
        return locationKey;
    } else {
        http.end();
        return "";
    }
}

void getWeatherInfo(AsyncWebServerRequest *request) {
    String locationKey = getLocationKey();
    if (locationKey == "") {
        request->send(500, "text/plain", "Failed to retrieve location key");
        return;
    }

    HTTPClient http;
    String serverPath = String(weatherServerName) + locationKey + "?apikey=" + apiKey;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        float temperature = doc[0]["Temperature"]["Metric"]["Value"];
        const char* condition = doc[0]["WeatherText"];

        String weather = "Temperature: " + String(temperature) + "°C, Condition: " + String(condition);
        request->send(200, "text/plain", weather);
    } else {
        request->send(500, "text/plain", "Failed to retrieve weather information");
    }

    http.end();
}

void connectToInternetRadioStation(AsyncWebServerRequest *request) {
    // TODO
    String message = "Connecting to Internet Radio Station...";
    request->send(200, "text/plain", message);
}