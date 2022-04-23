#ifndef VERB69_H
#define VERB69_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

class Verb69
{
public:
  Verb69(AlarmIndicator &ai, DigitalIndicator &di);
  virtual ~Verb69();

private:
  AlarmIndicator m_ai;
  DigitalIndicator m_di;
};

#endif
