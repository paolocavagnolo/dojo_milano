#include <Controllino.h>

#define BTN_EXT CONTROLLINO_A5
#define BTN_INT CONTROLLINO_A0

#define REL_SERRATURA CONTROLLINO_D0
#define REL_LUCE_BLU CONTROLLINO_D4


void setup() {

  pinMode(BTN_EXT, INPUT);
  pinMode(BTN_INT, INPUT);
  pinMode(REL_SERRATURA, OUTPUT);
  pinMode(REL_LUCE_BLU, OUTPUT);

  delay(500);

  Serial.begin(9600);

  delay(500);
}

unsigned long tExt = 0;
unsigned long tInt = 0;
unsigned long tSerr = 0;

bool sExt = false;
bool sInt = false;

bool pExt = false;
bool pInt = false;
bool pSerr = false;

void loop() {

  if (!sExt) {
    if ((millis() - tExt) > 300) {
      if (analogRead(BTN_EXT) > 400) {
        tExt = millis();
        sExt = true;

        pExt = true;
      }
    }
  } else {
    if ((millis() - tExt) > 300) {
      if (analogRead(BTN_EXT) < 100) {
        tExt = millis();
        sExt = false;
      }
    }
  }

  if (!sInt) {
    if ((millis() - tInt) > 300) {
      if (analogRead(BTN_INT) > 400) {
        tInt = millis();
        sInt = true;

        pInt = true;
      }
    }
  } else {
    if ((millis() - tInt) > 300) {
      if (analogRead(BTN_INT) < 100) {
        tInt = millis();
        sInt = false;
      }
    }
  }

  if (pExt) {
    pExt = false;
    digitalWrite(REL_LUCE_BLU, HIGH);
  }

  if (pInt) {
    pInt = false;
    digitalWrite(REL_LUCE_BLU, LOW);

    pSerr = true;
    digitalWrite(REL_SERRATURA, HIGH);
    tSerr = millis();
  }

  if (pSerr) {
    if ((millis() - tSerr) > 800) {
      digitalWrite(REL_SERRATURA, LOW);
      pSerr = false;
    }
  }

}
