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
#include "Program00.h"
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

TFT_eSPI tft = TFT_eSPI(320, 240);
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

struct program programs[] = {
  {  0, program00_start, program00_cycle },
  { -1,               0,               0 }
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

void startUp(void)
{
  for (int reg = 0; reg < 4; reg++) {
    uint32_t value = 0;
    for (int i = 0; i < 6; i++) {
      value = value + 8 * pow(10, i);
      if (reg == 0) {
        digitalInd->setRegister1(value);
      } else if (reg == 1) {
        digitalInd->setRegister2(value);
      } else {
        digitalInd->setRegister3(value);        
      }
      delay(25);
    }  
  }
  delay(1000);
  digitalInd->setComputerActivityStatus(false);
  alarmInd->setTemperatureCaution(false);
  alarmInd->setGimbalLockStatus(false);
  alarmInd->setProgramCondition(false);
  alarmInd->setTrackerCondition(false);
  alarmInd->setAltitudeDataCaution(false);
  alarmInd->setVelocityDataCaution(false);
  alarmInd->setOperatorErrorStatus(false);
  alarmInd->setKeyReleaseStatus(false);
  alarmInd->setStandbyStatus(false);
  alarmInd->setUplinkActivityStatus(false);
  digitalInd->setProgramNumber("00");
  digitalInd->setVerbCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  digitalInd->setNounCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  digitalInd->setRegister1(DIGITAL_INDICATOR_REGISTER_VALUE_NAN);
  digitalInd->setRegister2(DIGITAL_INDICATOR_REGISTER_VALUE_NAN);
  digitalInd->setRegister3(DIGITAL_INDICATOR_REGISTER_VALUE_NAN);        
}

void setup() {
  char dns[30];

  Serial.begin(115200);

  pinMode(ALARM_INDICATOR_CS, OUTPUT);
  digitalWrite(ALARM_INDICATOR_CS, HIGH);

  pinMode(DIGITAL_INDICATOR_CS, OUTPUT);
  digitalWrite(DIGITAL_INDICATOR_CS, HIGH);

  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  delay(5);
  digitalWrite(0, LOW);
  delay(20);
  digitalWrite(0, HIGH);

  // Initialize both displays
  tft.TFT_CS_MASK = (1 << ALARM_INDICATOR_CS) | (1 << DIGITAL_INDICATOR_CS);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Backlight
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  digitalInd = new DigitalIndicator(&tft, &spr);
  alarmInd = new AlarmIndicator(&tft);
  kbd = new Kbd(alarmInd, digitalInd);

  startUp();

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

    if ((verbCode != 0) && (*startFn == 0))
    {
      Serial.print("OPR ERR: verb:");
      Serial.print(verbCode);
      Serial.print(" noun:");
      Serial.println(nounCode);
      alarmInd->setOperatorErrorStatusBlinking();
    }
  }
}

startFn_t programStartFn = 0;
cycleFn_t programCycleFn = 0;

void findProgramStartCycleFunctions(int8_t programNumber, startFn_t *startFn, cycleFn_t *cycleFn)
{
  if (programNumber != PROGRAM_NUMBER_INVALID) {
    for (uint8_t program = 0; programs[program].number != PROGRAM_NUMBER_INVALID; program++) {
      if (programs[program].number == programNumber) {
          *startFn = programs[program].startFn;
          *cycleFn = programs[program].cycleFn;
          break;
      }
    }

    if ((programNumber != 0) && (*startFn == 0))
    {
      Serial.print("PROGRAM: program not found:");
      Serial.println(programNumber);
      alarmInd->setProgramCondition(true);
    }
  }
}

void loop() {
  uint8_t next_state = state;

  switch (state) {
    case FAGC_INIT:
      {
        startFn = 0;
        cycleFn = 0;
        findProgramStartCycleFunctions(0, &programStartFn, &programCycleFn);
        next_state = FAGC_IDLE;
        break;
      }
    case FAGC_IDLE:
      {
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

  if (programStartFn) {
    programStartFn(alarmInd, digitalInd, weather);
    programStartFn = 0;
  }

  if (programCycleFn) {
    programCycleFn(false);
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
