#ifndef VERB35_H
#define VERB35_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

class Verb35
{
public:
  Verb35(AlarmIndicator &ai, DigitalIndicator &di);
  virtual ~Verb35();

private:
  AlarmIndicator m_ai;
  DigitalIndicator m_di;
};

#endif
