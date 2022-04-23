#include <ESPAsyncWebServer.h>
#ifdef ESP32
  #include <ESPmDNS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#endif
#include <AsyncWebConfig.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "OTA.h"

AlarmIndicator *alarmInd;
DigitalIndicator *digitalInd;
#ifdef ESP32
OTA *ota;
#endif

TFT_eSPI tft = TFT_eSPI(320, 480);
TFT_eSprite spr = TFT_eSprite(&tft);

AsyncWebServer server(80);
AsyncWebConfig conf;

String params = "["
  "{"
  "'name':'ssid',"
  "'label':'WLAN ESSID',"
  "'type':"+String(INPUTTEXT)+","
  "'default':''"
  "},"
  "{"
  "'name':'pwd',"
  "'label':'WLAN Password',"
  "'type':"+String(INPUTPASSWORD)+","
  "'default':''"
  "}"
"]";

boolean initWiFi() {
    boolean connected = false;
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(conf.values[0]);
    if (conf.values[0] != "") {
      WiFi.begin(conf.values[0].c_str(), conf.values[1].c_str());
      uint8_t cnt = 0;

      while ((WiFi.status() != WL_CONNECTED) && (cnt < 20)) {
        delay(500);
        Serial.print(".");
        cnt++;
      }

      Serial.println();

      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP-Address = ");
        Serial.println(WiFi.localIP());
        connected = true;
      }
    }

    if (!connected) {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(conf.getApName(), "", 1);
    }

    return connected;
}

void handleRoot(AsyncWebServerRequest *request) {
  conf.handleFormRequest(request);

  if (request->hasParam("SAVE")) {
    uint8_t cnt = conf.getCount();

    Serial.println("*********** Configuration ************");

    for (uint8_t i = 0; i < cnt; i++) {
      Serial.print(conf.getName(i));
      Serial.print(" = ");
      Serial.println(conf.values[i]);
    }
  }
}

void setup() {
  char dns[30];

  Serial.begin(115200);

  pinMode(ALARM_INDICATOR_CS, OUTPUT);
  digitalWrite(ALARM_INDICATOR_CS, HIGH);

  pinMode(DIGITAL_INDICATOR_CS, OUTPUT);
  digitalWrite(DIGITAL_INDICATOR_CS, HIGH);

  // Initialize both displays
  digitalWrite(ALARM_INDICATOR_CS, LOW);
  digitalWrite(DIGITAL_INDICATOR_CS, LOW);
  tft.init();
  digitalWrite(ALARM_INDICATOR_CS, HIGH);
  digitalWrite(DIGITAL_INDICATOR_CS, HIGH);

//  alarmInd = new AlarmIndicator(&tft);
//  alarmInd->setProgramCondition(true);

  digitalInd = new DigitalIndicator(&tft, &spr);

  conf.setDescription(params);
  conf.readConfig();

  initWiFi();

  sprintf(dns,"%s.local",conf.getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder started");
  }

#ifdef EPS32
  ota = new OTA(server);
#endif

  server.on("/", handleRoot);
  server.begin();
}

bool acty = false;
uint8_t actyCounter = 0;

void loop() {

  if (!acty) {
    if (actyCounter > 7) {
      acty = !acty;
      actyCounter = 0;
      digitalInd->setComputerActivityStatus(acty);
    }
  } else {
    if (actyCounter > 3) {
      acty = !acty;
      actyCounter = 0;
      digitalInd->setComputerActivityStatus(acty);
    }    
  }

#ifndef ESP32
  MDNS.update();
#endif

  delay(250);
  actyCounter++;
}
