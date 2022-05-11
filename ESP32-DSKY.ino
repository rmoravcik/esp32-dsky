#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <AsyncWebConfig.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Kbd.h"
#include "Verb05.h"
#include "Verb06.h"
#include "Verb16.h"
#include "Verb35.h"
#include "Verb36.h"
#include "Verb37.h"
#include "Verb69.h"
#include "OTA.h"
#include "RTC.h"
#include "Weather.h"

#include "ESP32-DSKY.h"

DSKY dsky;

TFT_eSPI tft = TFT_eSPI(320, 240);
TFT_eSprite spr = TFT_eSprite(&tft);

AsyncWebServer server(80);
AsyncWebConfig conf;

struct noun verb05nouns[] = {
  {  9, verb05noun09_start, verb05noun09_cycle },
  { -1,                  0,                  0 }
};

struct noun verb06nouns[] = {
  { 43, verb06noun43_start, verb06noun43_cycle },
  { 95, verb06noun95_start, verb06noun95_cycle },
  { -1,                  0,                  0 }
};

struct noun verb16nouns[] = {
  { 36, verb16noun36_start, verb16noun36_cycle },
  { -1,                  0,                  0 }
};

struct noun verb37nouns[] = {
  {  0,       verb37noun00_start,       verb37noun00_cycle },
  {  6,       verb37noun06_start,       verb37noun06_cycle },
  { -1,                        0,                        0 }
};

struct verb verbs[] = {
  { 05,  true,            0,            0, verb05nouns },
  { 06,  true,            0,            0, verb06nouns },
  { 16,  true,            0,            0, verb16nouns },
  { 35, false, verb35_start, verb35_cycle,           0 },
  { 36, false, verb36_start, verb36_cycle,           0 },
  { 37,  true,            0,            0, verb37nouns },
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
        dsky.di->setComputerActivityStatus(true);
        delay(250);
        dsky.di->setComputerActivityStatus(false);
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
      dsky.ai->setProgramCondition(true);
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
  for (int reg = 0; reg < 3; reg++) {
    String value = "+";
    for (int i = 0; i < 6; i++) {
      value = value + "8";
      if (reg == 0) {
        dsky.di->setRegister1(value);
      } else if (reg == 1) {
        dsky.di->setRegister2(value);
      } else {
        dsky.di->setRegister3(value);
      }
      delay(25);
    }  
  }
  delay(1000);
  dsky.di->setComputerActivityStatus(false);
  dsky.ai->setTemperatureCaution(false);
  dsky.ai->setGimbalLockStatus(false);
  dsky.ai->setProgramCondition(false);
  dsky.ai->setTrackerCondition(false);
  dsky.ai->setAltitudeDataCaution(false);
  dsky.ai->setVelocityDataCaution(false);
  dsky.ai->setOperatorErrorStatus(false);
  dsky.ai->setKeyReleaseStatus(false);
  dsky.ai->setStandbyStatus(false);
  dsky.ai->setUplinkActivityStatus(false);
  dsky.di->setProgramNumber(DIGITAL_INDICATOR_VALUE_NAN);
  dsky.di->setVerbCode(DIGITAL_INDICATOR_VALUE_NAN);
  dsky.di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
  dsky.di->setRegister1(DIGITAL_INDICATOR_VALUE_NAN);
  dsky.di->setRegister2(DIGITAL_INDICATOR_VALUE_NAN);
  dsky.di->setRegister3(DIGITAL_INDICATOR_VALUE_NAN);
}

void setup() {
  char dns[30];

  pinMode(GPIO_BACKLIGHT, INPUT);
  Serial.begin(115200);

  pinMode(GPIO_ALARM_INDICATOR_CS, OUTPUT);
  digitalWrite(GPIO_ALARM_INDICATOR_CS, HIGH);

  pinMode(GPIO_DIGITAL_INDICATOR_CS, OUTPUT);
  digitalWrite(GPIO_DIGITAL_INDICATOR_CS, HIGH);

  pinMode(GPIO_TFT_RST, OUTPUT);
  digitalWrite(GPIO_TFT_RST, HIGH);
  delay(5);
  digitalWrite(GPIO_TFT_RST, LOW);
  delay(20);
  digitalWrite(GPIO_TFT_RST, HIGH);

  // Initialize both displays
  tft.TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS) | (1 << GPIO_DIGITAL_INDICATOR_CS);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Backlight
  ledcSetup(0, 5000, 8);
  ledcAttachPin(GPIO_BACKLIGHT, 0);
  ledcWrite(0, 200);

  dsky.di = new DigitalIndicator(&tft, &spr);
  dsky.ai = new AlarmIndicator(&tft);
  dsky.kbd = new Kbd(dsky.ai, dsky.di);

  startUp();

  conf.setDescription(params);
  conf.readConfig();

  initWiFi();

  sprintf(dns,"%s",conf.getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder started");
  }

  dsky.ota = new OTA(&server, dsky.ai);

  server.on("/", handleRoot);
  server.begin();

  dsky.rtc = new RTC(conf.getString("ntp_server"),
                conf.getInt("time_zone"),
                dsky.ai);

  dsky.weather = new Weather(conf.getString("weather_city"),
                        conf.getString("weather_country"),
                        conf.getString("weather_api_key"),
                        dsky.ai);
}

uint8_t state = DSKY_STATE_INIT;

startFn_t startFn = 0;
cycleFn_t cycleFn = 0;
extern cycleFn_t programCycleFn;

void findStartCycleFunctions(int8_t verbCode, int8_t nounCode, startFn_t *startFn, cycleFn_t *cycleFn)
{
  bool nounCodeMissing = false;

  if ((verbCode == VERB_CODE_INVALID) && (nounCode != NOUN_CODE_INVALID)) {
    if (dsky.di->getVerbCode() != DIGITAL_INDICATOR_VALUE_NAN) {
      verbCode = dsky.di->getVerbCode().toInt();
    }
  }

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
          } else {
            dsky.di->setVerbCodeBlinking(true);
            nounCodeMissing = true;
          }
        } else {
          *startFn = verbs[verb].startFn;
          *cycleFn = verbs[verb].cycleFn;
          break;
        }
      }
    }

    if ((!nounCodeMissing) && (verbCode != VERB_CODE_INVALID) && (*startFn == 0))
    {
      Serial.print("OPR ERR: verb:");
      Serial.print(verbCode);
      Serial.print(" noun:");
      Serial.println(nounCode);
      dsky.ai->setOperatorErrorStatusBlinking();
    }
  }
}

void loop() {
  uint8_t next_state = state;

  char key = dsky.kbd->update();

  switch (state) {
    case DSKY_STATE_INIT:
      {
        startFn = 0;
        cycleFn = 0;
        verb37noun00_start(&dsky);
        dsky.di->setVerbCode("37");
        dsky.di->setVerbCodeBlinking(true);
        dsky.ai->setRestartCondition(true);
        next_state = DSKY_STATE_IDLE;
        break;
      }
    case DSKY_STATE_IDLE:
      {
        findStartCycleFunctions(dsky.kbd->getVerbCode(), dsky.kbd->getNounCode(), &startFn, &cycleFn);

        if (startFn) {
          next_state = startFn(&dsky);
          startFn = 0;
        }
        break;
      }
    case DSKY_STATE_BUSY:
      {
        startFn_t newStartFn = 0;
        cycleFn_t newCycleFn = 0;

        findStartCycleFunctions(dsky.kbd->getVerbCode(), dsky.kbd->getNounCode(), &newStartFn, &newCycleFn);

        if (cycleFn) {
          next_state = cycleFn(key, newStartFn ? true : false, next_state);

          if (next_state != DSKY_STATE_BUSY) {
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

  if (programCycleFn) {
    next_state = programCycleFn(key, false, next_state);
  }

  if (next_state != state) {
      Serial.print("Next state:");
      Serial.print(next_state);
      Serial.print(" previous state:");
      Serial.println(state);
      state = next_state;
  }

  dsky.ai->update();
  dsky.di->update();
  dsky.weather->update();

  delay(MAIN_LOOP_DELAY_MS);
}
