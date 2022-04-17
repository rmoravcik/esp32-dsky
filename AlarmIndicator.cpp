#include "AlarmIndicator.h"

AlarmIndicator::AlarmIndicator(TFT_eSPI &tft) : m_tft(tft)
{
};

AlarmIndicator::~AlarmIndicator()
{
};

void AlarmIndicator::setUplinkActivityStatus(bool status)
{
}

void AlarmIndicator::setNoAttitudeStatus(bool status)
{
}

void AlarmIndicator::setStandbyStatus(bool status)
{
}

void AlarmIndicator::setKeyReleaseStatus(bool status)
{
}

void AlarmIndicator::setOperatorErrorStatus(bool status)
{
}

void AlarmIndicator::setTemperatureCaution(bool status)
{
}

void AlarmIndicator::setGimbalLockStatus(bool status)
{
}

void AlarmIndicator::setProgramCondition(bool status)
{
}

void AlarmIndicator::setRestartCondition(bool status)
{
}

void AlarmIndicator::setTrackerCondition(bool status)
{
}

void AlarmIndicator::setAltitudeDataCaution(bool status)
{
}

void AlarmIndicator::setVelocityDataCaution(bool status)
{
}
