#ifndef OTA_H
#define OTA_H

#include <ESPAsyncWebServer.h>

#include "AlarmIndicator.h"

class OTA
{
public:
  OTA(AsyncWebServer *server, AlarmIndicator *ai);
  virtual ~OTA();

private:
  AsyncWebServer *m_server;
  AlarmIndicator *m_ai;
};

#endif /* OTA_H */
