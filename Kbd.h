#ifndef KBD_H
#define KBD_H

#include <Keypad.h>

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

#define KEY_PRO 'p'

class Kbd
{
public:
  Kbd(AlarmIndicator *ai, DigitalIndicator *di);
  virtual ~Kbd();

  int8_t getVerbCode(void);
  int8_t getNounCode(void);

  char update(void);

  Keypad *m_keypad;

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  bool m_verbKeyPressed;
  bool m_nounKeyPressed;

  String m_verbCode;
  String m_nounCode;
};

#endif /* KBD_H */
