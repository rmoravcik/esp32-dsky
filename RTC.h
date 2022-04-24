#ifndef RTC_H
#define RTC_H

#include <WiFiUdp.h>
#include <TimeLib.h>

#define NTP_PACKET_SIZE 48 // NTP time is in the first 48 bytes of message

class RTC
{
public:
  RTC(const String ntpServer, int timeZone);
  virtual ~RTC();

  int8_t m_timeZone;
  String m_ntpServer;

  uint8_t dstOffset(uint8_t d, uint8_t m, uint32_t y, uint8_t h);
  time_t getNtpTime(void);

private:
  uint8_t m_packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

  WiFiUDP m_udp;

  void sendNTPpacket(IPAddress &address);
};

#endif /* RTC_H */
