#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "ESP32-DSKY.h"
#include "Weather.h"

Weather::Weather(const String city, const String country, const String apiKey, AlarmIndicator *ai)
{
  m_city = city;
  m_country = country;
  m_apiKey = apiKey;

  m_longitude = 0;
  m_latitude = 0;
  m_temperature = 0;
  m_pressure = 0;
  m_humidity = 0;

  m_ai = ai;

  m_lastUpdate = 0;
  m_updateFailed = false;
}

Weather::~Weather()
{
}

bool Weather::decodeWeather(WiFiClient& json)
{
  DynamicJsonDocument doc(1024);
  float value;

  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("DeserializeJson() failed: "));
    Serial.println(error.c_str());
    m_updateFailed = true;
    m_ai->setProgramCondition(true);
    return false;
  }

  JsonObject root = doc.as<JsonObject>();

  Serial.println("Decoding weather:");

  value = root["coord"]["lon"].as<float>();
  Serial.println(" Longitude: " + String(value));
  m_longitude = (uint16_t)(value * 100);

  value = root["coord"]["lat"].as<float>();
  Serial.println(" Latitude: "+ String(value));
  m_latitude = (uint16_t)(value * 100);

  value = root["main"]["temp"].as<float>();
  Serial.println("Temperature: "+ String(value));
  m_temperature = (uint16_t)(value * 100);

  value = root["main"]["pressure"].as<float>();
  Serial.println("Pressure: "+ String(value));
  m_pressure = (uint16_t)value;

  value = root["main"]["humidity"].as<float>();
  Serial.println("Humidity: "+ String(value));
  m_humidity = (uint16_t)value;

  m_updateFailed = true;

  return true;
}

bool Weather::update()
{
  if ((now() - m_lastUpdate) > WEATHER_UPDATE_S) {
      HTTPClient http;
      String uri = "/data/2.5/weather?q=" + m_city + "," + m_country + "&APPID=" + m_apiKey + "&mode=json&units=metric";

      m_lastUpdate = now();
      m_client.stop(); // close connection before sending a new request

      Serial.print("Updating weather ");
      Serial.print(m_server);
      Serial.println(uri);

      http.begin(m_client, m_server, 80, uri);
      int httpCode = http.GET();

      if(httpCode == HTTP_CODE_OK) {
        if (!decodeWeather(http.getStream())) {
          m_updateFailed = true;
          m_ai->setProgramCondition(true);
          return false;
        }

        m_client.stop();
        http.end();

        if (m_temperature < 1000) {
          m_ai->setTemperatureCaution(true);
        } else {
          m_ai->setTemperatureCaution(false);
        }

        m_ai->setAltitudeDataCaution(true);
        return true;
      } else {
        Serial.printf("Connection failed, error: %s", http.errorToString(httpCode).c_str());
        m_client.stop();
        http.end();
        m_updateFailed = true;
        m_ai->setProgramCondition(true);
        return false;
      }

      http.end();
  }

  return true;
}
