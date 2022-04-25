#ifndef WEATHER_H
#define WEATHER_H

#include <WiFiClient.h>
#include <TimeLib.h>

class Weather
{
public:
  Weather(const String city, const String country, const String apikey);
  virtual ~Weather();

  bool update(void);

  uint16_t getLongitude(void) { return m_longitude; };
  uint16_t getLatitude(void) { return m_latitude; };
  uint16_t getTemperature(void) { return m_temperature; };
  uint16_t getPressure(void) { return m_pressure; };
  uint16_t getHumidity(void) { return m_humidity; };

private:
  WiFiClient m_client;

  time_t m_lastUpdate;

  const String m_server = "api.openweathermap.org";
  String m_city;
  String m_country;
  String m_apiKey;

  uint16_t m_longitude;
  uint16_t m_latitude;
  uint16_t m_temperature;
  uint16_t m_pressure;
  uint16_t m_humidity;

  bool decodeWeather(WiFiClient& json);
};

#endif /* WEATHER_H */
