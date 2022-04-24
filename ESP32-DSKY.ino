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
#include "Verb35.h"
#include "Verb69.h"
#include "OTA.h"

#include "ESP32-DSKY.h"

AlarmIndicator *alarmInd;
DigitalIndicator *digitalInd;
#ifdef ESP32
OTA *ota;
#endif

TFT_eSPI tft = TFT_eSPI(320, 480);
TFT_eSprite spr = TFT_eSprite(&tft);

AsyncWebServer server(80);
AsyncWebConfig conf;

struct verb verbs[VERB_COUNT] = {
  { 35, verb35_start, verb35_cycle },
  { 69, verb69_start, verb69_cycle }
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
int8_t verbCode = VERB_CODE_INVALID;
uint8_t actyVerb = VERB_UNSET;

// REMOVE ME
uint32_t start_verb35 = 1000;

void loop() {
  uint8_t next_state = state;

  switch (state) {
    case FAGC_INIT:
      {
        alarmInd->setProgramCondition(true);
        next_state = FAGC_IDLE;
        break;
      }
    case FAGC_IDLE:
      {
        next_state = acty_cycle();

        // REMOVE ME
        if (start_verb35 > 0)
        {
          if (start_verb35 == 200) {
            digitalInd->setVerbCode(3);
          }
          if (start_verb35 == 100) {
            digitalInd->setVerbCode(35);
          }
          if (start_verb35 == 1) {
            verbCode = 35;
          }
          start_verb35--;
        }

        for (uint8_t i = 0; i < VERB_COUNT; i ++) {
          if (verbs[i].code == verbCode) {
            actyVerb = i;
            verbCode = VERB_CODE_INVALID;
            break;
          }
        }

        if ((verbCode != VERB_CODE_INVALID) && (actyVerb == VERB_UNSET))
        {
          alarmInd->setOperatorErrorStatus(true);
        }

        if (actyVerb != VERB_UNSET) {
          next_state = verbs[actyVerb].startFn(alarmInd, digitalInd);
        }
        break;
      }
    case FAGC_BUSY:
      {
        if (actyVerb != VERB_UNSET) {
          next_state = verbs[actyVerb].cycleFn();
          if (next_state == FAGC_IDLE) {
            actyVerb = VERB_UNSET;
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

#ifndef ESP32
  MDNS.update();
#endif

  delay(MAIN_LOOP_DELAY_MS);
}
