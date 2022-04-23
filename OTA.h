#ifndef OTA_H
#define OTA_H

#ifdef ESP32

#include <ESPAsyncWebServer.h>

class OTA
{
public:
  OTA(AsyncWebServer &server);
  virtual ~OTA();

private:
  AsyncWebServer m_server;
};

#endif

#endif
