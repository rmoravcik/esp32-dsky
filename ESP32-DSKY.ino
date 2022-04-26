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
#include "Kbd.h"
#include "Verb06.h"
#include "Verb16.h"
#include "Verb35.h"
#include "Verb36.h"
#include "Verb69.h"
#include "OTA.h"
#include "RTC.h"
#include "Weather.h"

#include "ESP32-DSKY.h"

AlarmIndicator *alarmInd;
DigitalIndicator *digitalInd;
Kbd *kbd;
#ifdef ESP32
OTA *ota;
#endif
RTC *rtc;
Weather *weather;

TFT_eSPI tft = TFT_eSPI(320, 480);
TFT_eSprite spr = TFT_eSprite(&tft);

AsyncWebServer server(80);
AsyncWebConfig conf;

struct noun verb06nouns[] = {
  { 43, verb06noun43_start, verb06noun43_cycle },
  { 95, verb06noun95_start, verb06noun95_cycle },
  { -1,                  0,                  0 }
};

struct noun verb16nouns[] = {
  { 36, verb16noun36_start, verb16noun36_cycle },
  { -1,                  0,                  0 }
};

struct verb verbs[] = {
  { 06,  true,            0,            0, verb06nouns },
  { 16,  true,            0,            0, verb16nouns },
  { 35, false, verb35_start, verb35_cycle,           0 },
  { 36, false, verb36_start, verb36_cycle,           0 },
  // 37
  { 69, false, verb69_start, verb69_cycle,           0 },
  { -1, false,            0,            0,           0 }
};

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
  "},"
  "{"
  "'name':'ntp_server',"
  "'label':'NTP Server',"
  "'type':"+String(INPUTTEXT)+","
  "'default':'pool.ntp.org'"
  "},"
  "{"
  "'name':'time_zone',"
  "'label':'Time zone',"
  "'type':"+String(INPUTTEXT)+","
  "'default':'1'"
  "},"
  "{"
  "'name':'weather_city',"
  "'label':'City',"
  "'type':"+String(INPUTTEXT)+","
  "'default':''"
  "},"
  "{"
  "'name':'weather_country',"
  "'label':'Country',"
  "'type':"+String(INPUTTEXT)+","
  "'default':''"
  "},"
  "{"
  "'name':'weather_api_key',"
  "'label':'OpenWeatherMap API Key',"
  "'type':"+String(INPUTTEXT)+","
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
        digitalInd->setComputerActivityStatus(true);
        delay(250);
        digitalInd->setComputerActivityStatus(false);
        delay(250);
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

  alarmInd = new AlarmIndicator(&tft);
  digitalInd = new DigitalIndicator(&tft, &spr);
  kbd = new Kbd(alarmInd, digitalInd);

  conf.setDescription(params);
  conf.readConfig();

  initWiFi();

  sprintf(dns,"%s",conf.getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder started");
  }

#ifdef ESP32
  ota = new OTA(&server);
#endif

  server.on("/", handleRoot);
  server.begin();

  rtc = new RTC(conf.getString("ntp_server"),
                conf.getInt("time_zone"));

  weather = new Weather(conf.getString("weather_city"),
                        conf.getString("weather_country"),
                        conf.getString("weather_api_key"));
}

bool acty = false;
uint8_t actyCounter = 0;

uint8_t acty_cycle(void) {
  if (!acty) {
    if (actyCounter > ACTY_OFF_DELAY_MS) {
      acty = !acty;
      actyCounter = 0;
      digitalInd->setComputerActivityStatus(acty);
    }
  } else {
    if (actyCounter > ACTY_ON_DELAY_MS) {
      acty = !acty;
      actyCounter = 0;
      digitalInd->setComputerActivityStatus(acty);
    }
  }
  actyCounter++;
  return FAGC_IDLE;
}

uint8_t state = FAGC_INIT;

startFn_t startFn = 0;
cycleFn_t cycleFn = 0;

void findStartCycleFunctions(int8_t verbCode, int8_t nounCode, startFn_t *startFn, cycleFn_t *cycleFn)
{
  if (verbCode != VERB_CODE_INVALID) {
    for (uint8_t verb = 0; verbs[verb].code != VERB_CODE_INVALID; verb++) {
      if (verbs[verb].code == verbCode) {
        if (verbs[verb].nounRequired) {
          if (nounCode != NOUN_CODE_INVALID) {
            for (uint8_t noun = 0; verbs[verb].nouns[noun].code != NOUN_CODE_INVALID; noun++) {
              if (verbs[verb].nouns[noun].code == nounCode) {
                *startFn = verbs[verb].nouns[noun].startFn;
                *cycleFn = verbs[verb].nouns[noun].cycleFn;
                break;
              }
            }                  
          }
        } else {
          *startFn = verbs[verb].startFn;
          *cycleFn = verbs[verb].cycleFn;
          break;
        }
      }
    }

    if (startFn == 0)
    {
      alarmInd->setOperatorErrorStatusBlinking();
    }
  }
}

void loop() {
  uint8_t next_state = state;

  switch (state) {
    case FAGC_INIT:
      {
        alarmInd->setProgramCondition(true);
        acty = false;
        actyCounter = 0;
        startFn = 0;
        cycleFn = 0;
        next_state = FAGC_IDLE;
        break;
      }
    case FAGC_IDLE:
      {
        next_state = acty_cycle();

        findStartCycleFunctions(kbd->getVerbCode(), kbd->getNounCode(), &startFn, &cycleFn);

        if (startFn) {
          next_state = startFn(alarmInd, digitalInd, weather);
          startFn = 0;
        }
        break;
      }
    case FAGC_BUSY:
      {
        startFn_t newStartFn = 0;
        cycleFn_t newCycleFn = 0;

        findStartCycleFunctions(kbd->getVerbCode(), kbd->getNounCode(), &newStartFn, &newCycleFn);

        if (cycleFn) {
          next_state = cycleFn(newStartFn ? true : false);

          if (next_state != FAGC_BUSY) {
            if (newStartFn) {
              startFn = newStartFn;
              cycleFn = newCycleFn;
            } else {
              cycleFn = 0;
            }
          }
        }
        break;
      }
    default:
      Serial.print("ERROR: Unknown state:");
      Serial.println(state);
      break;
  }

  if (next_state != state) {
      Serial.print("Next state:");
      Serial.print(next_state);
      Serial.print(" previous state:");
      Serial.println(state);
      state = next_state;
  }

  alarmInd->update();
  kbd->update();
  weather->update();

#ifndef ESP32
  MDNS.update();
#endif

  delay(MAIN_LOOP_DELAY_MS);
}
