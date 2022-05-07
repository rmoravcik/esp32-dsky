#include "ESP32-DSKY.h"
#include "Kbd.h"

const byte ROWS = 3;
const byte COLS = 7;

char keys[ROWS][COLS] = {
  { KEY_NONE, '+', '7', '8', '9', KEY_CLR, KEY_NONE },
  { KEY_VERB, '-', '4', '5', '6', KEY_PRO, KEY_ENTR },
  { KEY_NOUN, '0', '1', '2', '3', KEY_REL, KEY_RSET },
};

byte rowPins[ROWS] = { 4, 39, 34 };
byte colPins[COLS] = { 15, 33, 21, 22, 32, 25, 27 };

static Kbd *inst = NULL;

void keypadEvent(KeypadEvent key)
{
  switch (inst->m_keypad->getState()) {
    case RELEASED:
      {
        Serial.print("key=");
        Serial.println(key);
        switch (key) {
          case KEY_VERB:
            {
              inst->m_di->setVerbCodeBlinking(false);
              inst->m_di->setNounCodeBlinking(false);
              inst->m_di->setVerbCode(DIGITAL_INDICATOR_VALUE_NAN);
              inst->m_verbKeyPressed = true;
              inst->m_nounKeyPressed = false;
            }
            break;
          case KEY_NOUN:
            {
              inst->m_verbKeyPressed = false;
              inst->m_nounKeyPressed = true;
              inst->m_di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
            }
            break;
          case KEY_ENTR:
            {
              inst->m_verbKeyPressed = false;
              inst->m_nounKeyPressed = false;
              inst->m_di->setVerbCodeBlinking(false);
            }
            break;
          case KEY_REL:
            {
              inst->m_ai->setKeyReleaseStatus(false);
            }
            break;
          case KEY_RSET:
            {
              inst->m_verbCode = "";
              inst->m_nounCode = "";
              inst->m_verbKeyPressed = false;
              inst->m_nounKeyPressed = false;
              inst->m_ai->resetIndicator();
            }
            break;
          default:
            {
              if (isdigit(key)) {
                if (inst->m_verbKeyPressed) {
                  if (inst->m_verbCode.length() < 2) {
                      inst->m_verbCode += key;
                      inst->m_di->setVerbCode(inst->m_verbCode);
                  } else {
                      Serial.println("OPR ERR: verbCode full");
                      inst->m_ai->setOperatorErrorStatusBlinking();
                  }
                }

                else if (inst->m_nounKeyPressed) {
                  if (inst->m_nounCode.length() < 2) {
                      inst->m_nounCode += key;
                      inst->m_di->setNounCode(inst->m_nounCode);
                  } else {
                      Serial.println("OPR ERR: nounCode full");
                      inst->m_ai->setOperatorErrorStatusBlinking();
                  }
                }

                // Allow to enter program number in sequence: V37E 06E
                else if (inst->m_di->getVerbCode() == "37") {
                  inst->m_nounKeyPressed = true;
                  if (inst->m_nounCode.length() < 2) {
                      inst->m_nounCode += key;
                      inst->m_di->setNounCode(inst->m_nounCode);
                  } else {
                      Serial.println("OPR ERR: nounCode full");
                      inst->m_ai->setOperatorErrorStatusBlinking();
                  }
                }
              }
            }
            break;
        }
      }
      break;
    default:
      break;
  }
}

Kbd::Kbd(AlarmIndicator *ai, DigitalIndicator *di)
{
  inst = this;

  m_ai = ai;
  m_di = di;

  m_verbKeyPressed = false;
  m_nounKeyPressed = false;

  m_keypad = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
  m_keypad->addEventListener(keypadEvent);
}

Kbd::~Kbd()
{
}

int8_t Kbd::getVerbCode(void)
{
  if (!m_verbKeyPressed && !m_nounKeyPressed) {
    if (m_verbCode.length() > 0) {
      int8_t ret = m_verbCode.toInt();
      m_verbCode = "";
      return ret;
    }
    return VERB_CODE_INVALID;
  } else {
    return VERB_CODE_INVALID;
  }
}

int8_t Kbd::getNounCode(void)
{
  if (!m_verbKeyPressed && !m_nounKeyPressed) {
    if (m_nounCode.length() > 0) {
      int8_t ret = m_nounCode.toInt();
      m_nounCode = "";
      return ret;
    }

    return NOUN_CODE_INVALID;
  } else {
    return NOUN_CODE_INVALID;
  }
}

char Kbd::update(void)
{
  return m_keypad->getKey();
}
