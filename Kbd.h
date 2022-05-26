#ifndef KBD_H
#define KBD_H

#include <Keypad.h>

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

#define KEY_NONE      ' '
#define KEY_VERB      'v'
#define KEY_NOUN      'n'
#define KEY_CLR       'c'
#define KEY_PRO       'p'
#define KEY_ENTR      'e'
#define KEY_REL       'k'
#define KEY_RSET      'r'

#define KEY_PRO_FORCE 'P'

class Kbd
{
public:
  Kbd(AlarmIndicator *ai, DigitalIndicator *di);
  virtual ~Kbd();

  int8_t getVerbCode(void);
  int8_t getNounCode(void);

  char update(void);
  KeyState getState(void) { return m_keypad->getState(); }

  Keypad *m_keypad;
  char m_holdKey;

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  bool m_verbKeyPressed;
  bool m_nounKeyPressed;

  String m_verbCode;
  String m_nounCode;
};

#endif /* KBD_H */
