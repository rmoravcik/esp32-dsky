#include "Verb69.h"

Verb69::Verb69(AlarmIndicator &ai, DigitalIndicator &di) : m_ai(ai), m_di(di)
{
  ESP.restart();
}

Verb69::~Verb69()
{
}
