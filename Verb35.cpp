#include "Verb35.h"

Verb35::Verb35(AlarmIndicator &ai, DigitalIndicator &di) : m_ai(ai), m_di(di)
{
  m_di.setProgramNumber(88);
  m_di.setVerbCode(88);
  m_di.setNounCode(88);

  m_di.setRegister1(88888);
  m_di.setRegister2(88888);
  m_di.setRegister3(88888);
}

Verb35::~Verb35()
{
}
