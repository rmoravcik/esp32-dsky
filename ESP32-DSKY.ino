#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include <soc/rtc_cntl_reg.h>

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "EnvSensor.h"
#include "Kbd.h"
#include "Program06.h"
#include "Verb05.h"
#include "Verb06.h"
#include "Verb16.h"
#include "Verb24.h"
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

struct noun verb05nouns[] = {
  {  9, verb05noun09_start, verb05noun09_cycle },
  { -1,                  0,                  0 }
};

struct noun verb06nouns[] = {
  { 34, verb06noun34_start, verb06noun34_cycle },
  { 43, verb06noun43_start, verb06noun43_cycle },
  { 95, verb06noun95_start, verb06noun95_cycle },
  { 96, verb06noun96_start, verb06noun96_cycle },
  { 99, verb06noun99_start, verb06noun99_cycle },
  { -1,                  0,                  0 }
};

struct noun verb16nouns[] = {
  { 36, verb16noun36_start, verb16noun36_cycle },
  { -1,                  0,                  0 }
};

struct noun verb24nouns[] = {
  { 34, verb24noun34_start, verb24noun34_cycle },
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
  { 24,  true,            0,            0, verb24nouns },
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
  "},"
  "{"
  "'name':'standby_start_time',"
  "'label':'Standy mode start time',"
  "'type':"+String(INPUTTIME)+","
  "'default':'22:00'"
  "}"
"]";

static boolean initWiFi() {
    boolean connected = false;
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(dsky.conf->values[0]);
    if (dsky.conf->values[0] != "") {
      WiFi.begin(dsky.conf->values[0].c_str(), dsky.conf->values[1].c_str());
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
      WiFi.softAP(dsky.conf->getApName(), "", 1);
      dsky.ai->setProgramCondition(true);
    }

    return connected;
}

static void handleRoot(AsyncWebServerRequest *request) {
  dsky.conf->handleFormRequest(request);

  if (request->hasParam("SAVE")) {
    uint8_t cnt = dsky.conf->getCount();

    Serial.println("*********** Configuration ************");

    for (uint8_t i = 0; i < cnt; i++) {
      Serial.print(dsky.conf->getName(i));
      Serial.print(" = ");
      Serial.println(dsky.conf->values[i]);
    }
  }
}

static void startUp(void)
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

  // Disable brownout detector
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  dsky.standbyMode = false;

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

  // TFT Backlight
  ledcSetup(0, 5000, 16);
  ledcAttachPin(GPIO_BACKLIGHT, 0);
  ledcWrite(0, 800);

  dsky.di = new DigitalIndicator(&tft, &spr);
  dsky.ai = new AlarmIndicator(&tft, &dsky.standbyMode);
  dsky.kbd = new Kbd(dsky.ai, dsky.di);

  startUp();

  dsky.conf = new AsyncWebConfig();
  dsky.conf->setDescription(params);
  dsky.conf->readConfig();

  initWiFi();

  sprintf(dns,"%s",dsky.conf->getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder started");
  }

  dsky.ota = new OTA(&server, dsky.ai);

  server.on("/", handleRoot);
  server.begin();

  dsky.rtc = new RTC(dsky.conf->getString("ntp_server"),
                     dsky.conf->getInt("time_zone"),
                     dsky.ai);

  dsky.weather = new Weather(dsky.conf->getString("weather_city"),
                             dsky.conf->getString("weather_country"),
                             dsky.conf->getString("weather_api_key"),
                             dsky.ai);

  dsky.sensor = new EnvSensor(dsky.ai);
}

uint8_t state = DSKY_STATE_INIT;

startFn_t startFn = 0;
cycleFn_t cycleFn = 0;
extern cycleFn_t programCycleFn;

static void findStartCycleFunctions(int8_t verbCode, int8_t nounCode, startFn_t *startFn, cycleFn_t *cycleFn)
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

static uint8_t checkStandyMode(cycleFn_t *cycleFn, cycleFn_t *programCycleFn, uint8_t state)
{
  if (!dsky.standbyMode) {
    static int8_t prevMinute = -1;
    uint8_t currMinute = minute();
    uint8_t stbHour, stbMinute;

    if (prevMinute != currMinute) {
      dsky.rtc->convertTime(dsky.conf->getString("standby_start_time"), &stbHour, &stbMinute);

      if ((stbHour == hour()) && (stbMinute == currMinute)) {
        program06_start(&dsky);
        state = program06_cycle(KEY_PRO_FORCE, false, state);
        *cycleFn = NULL;
        *programCycleFn = program06_cycle;
      }

      prevMinute = currMinute;
    }
  }
  return state;
}

void loop() {
  uint8_t next_state = state;

  char key = dsky.kbd->update();

  switch (state) {
    case DSKY_STATE_INIT:
      {
        if (WiFi.getMode() == WIFI_OFF) {
          initWiFi();
        }

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

  next_state = checkStandyMode(&cycleFn, &programCycleFn, next_state);

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
  dsky.sensor->update();

  delay(MAIN_LOOP_DELAY_MS);
}
