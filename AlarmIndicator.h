#ifndef ALARMINDICATOR_H
#define ALARMINDICATOR_H

#include <TFT_eSPI.h>

#define ALARM_INDICATOR_CS PIN_D0

class AlarmIndicator
{
public:
  AlarmIndicator(TFT_eSPI &tft);
  virtual ~AlarmIndicator();

  void setUplinkActivityStatus(bool status);
  void setNoAttitudeStatus(bool status);
  void setStandbyStatus(bool status);
  void setKeyReleaseStatus(bool status);
  void setOperatorErrorStatus(bool status);

  void setTemperatureCaution(bool status);
  void setGimbalLockStatus(bool status);
  void setProgramCondition(bool status);
  void setRestartCondition(bool status);
  void setTrackerCondition(bool status);
  void setAltitudeDataCaution(bool status);
  void setVelocityDataCaution(bool status);

private:
  TFT_eSPI m_tft;

  bool m_uplinkActivityStatus;
  bool m_noAttitudeStatus;
  bool m_standbyStatus;
  bool m_keyReleaseStatus;
  bool m_operatorErrorStatus;

  bool m_temperatureCaution;
  bool m_gimbalLockStatus;
  bool m_programCondition;
  bool m_restartCondition;
  bool m_trackerCondition;
  bool m_altitudeDataCaution;
  bool m_velocityDataCaution;
};

#endif
