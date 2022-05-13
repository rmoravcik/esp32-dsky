#include <WiFi.h>

#include "RTC.h"

static RTC *inst = NULL;

time_t getDstCorrectedTime(void)
{
  time_t t = inst->getNtpTime();

  if (t > 0) {
    TimeElements tm;
    breakTime(t, tm);
    t += (inst->m_timeZone + inst->dstOffset(tm.Day, tm.Month, tm.Year + 1970, tm.Hour)) * SECS_PER_HOUR;
  }

  return t;
}

RTC::RTC(const String ntpServer, int timeZone, AlarmIndicator *ai)
{
  inst = this;

  m_udp.begin(8888);

  m_ntpServer = ntpServer;
  m_timeZone = timeZone;
  m_ai = ai;
  m_updateFailed = false;

  setSyncProvider(getDstCorrectedTime);
  setSyncInterval(300);
}

RTC::~RTC()
{
}

uint8_t RTC::dstOffset(uint8_t d, uint8_t m, uint32_t y, uint8_t h)
{
  // Day in March that DST starts on, at 1 am
  uint8_t dstOn = (31 - (5 * y / 4 + 4) % 7);

  // Day in October that DST ends  on, at 2 am
  uint8_t dstOff = (31 - (5 * y / 4 + 1) % 7);

  if ((m > 3 && m < 10) ||
      (m == 3 && (d > dstOn || (d == dstOn && h >= 1))) ||
      (m == 10 && (d < dstOff || (d == dstOff && h <= 1))))
    return 1;
  else
    return 0;
}

time_t RTC::getNtpTime(void)
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (m_udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(m_ntpServer.c_str(), ntpServerIP);
  Serial.print(m_ntpServer);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = m_udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      m_udp.read(m_packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)m_packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)m_packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)m_packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)m_packetBuffer[43];
      m_updateFailed = false;
      return secsSince1900 - 2208988800UL;
    }
  }
  Serial.println("No NTP Response :-(");
  m_updateFailed = true;
  m_ai->setProgramCondition(true);
  return 0; // return 0 if unable to get the time
}

void RTC::sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(m_packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  m_packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  m_packetBuffer[1] = 0;     // Stratum, or type of clock
  m_packetBuffer[2] = 6;     // Polling Interval
  m_packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  m_packetBuffer[12] = 49;
  m_packetBuffer[13] = 0x4E;
  m_packetBuffer[14] = 49;
  m_packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  m_udp.beginPacket(address, 123); //NTP requests are to port 123
  m_udp.write(m_packetBuffer, NTP_PACKET_SIZE);
  m_udp.endPacket();
}

void RTC::convertTime(const String timeValue, uint8_t *h, uint8_t *m)
{
  bool separatorFound = false;
  String strHour = "";
  String strMinute = "";

  for (uint8_t index = 0; index < timeValue.length(); index++) {
    if (timeValue[index] == ':') {
      separatorFound = true;
      continue;
    }

    if (!separatorFound) {
      strHour = strHour + timeValue[index];
    } else {
      strMinute = strMinute + timeValue[index];
    }
  }

  *h = strHour.toInt();
  *m = strMinute.toInt();

  Serial.print("RTC::convertTime(): timeValue=");
  Serial.print(timeValue);
  Serial.print(" hour=");
  Serial.print(*h);
  Serial.print(" minute=");
  Serial.println(*m);
}
