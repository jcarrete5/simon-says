#include <WiFi101.h>
#include <EEPROM.h>

enum Sound {
  fail = 128,
  grn_frq = 165,
  blu_frq = 330,
  red_frq = 440,
  ylw_frq = 554
};

// Colors
const byte RED = 1;
const byte YELLOW = 2;
const byte BLUE = 3;
const byte GREEN = 4;

// Pins
const unsigned int BUZZER = 5;
const unsigned int GREEN_LED = 9;
const unsigned int BLUE_LED = 8;
const unsigned int RED_LED = 2;
const unsigned int YELLOW_LED = 4;
const unsigned int GREEN_BTN = A3;
const unsigned int BLUE_BTN = A2;
const unsigned int RED_BTN = A0;
const unsigned int YELLOW_BTN = A1;

const unsigned long BUZZ_DUR = 250; // ms

const unsigned int MAX_SEQ_LENGTH = 256;
byte sequence[MAX_SEQ_LENGTH];
unsigned int seqIndex = 0;

void buzz(unsigned int frq) {
  tone(BUZZER, frq, BUZZ_DUR);
  delay(BUZZ_DUR);
  delay(100);
}

void green() {
  digitalWrite(GREEN_LED, HIGH);
  buzz(grn_frq);
  digitalWrite(GREEN_LED, LOW);
}

void blue() {
  digitalWrite(BLUE_LED, HIGH);
  buzz(blu_frq);
  digitalWrite(BLUE_LED, LOW);
}

void red() {
  digitalWrite(RED_LED, HIGH);
  buzz(red_frq);
  digitalWrite(RED_LED, LOW);
}

void yellow() {
  digitalWrite(YELLOW_LED, HIGH);
  buzz(ylw_frq);
  digitalWrite(YELLOW_LED, LOW);
}

void nextColor() {
	byte randNum = floor(random(1, 5));
  sequence[seqIndex++] = randNum;
  EEPROM[0] = randNum;
}

void showPattern() {
  for (unsigned int i = 0; i < seqIndex; i++) {
    switch (sequence[i]) {
      case RED:
      	red();
      	break;
      case YELLOW:
      	yellow();
      	break;
      case BLUE:
      	blue();
      	break;
      case GREEN:
      	green();
      	break;
    }
    delay(200);
  }
}

bool checkInput() {
  for (unsigned int i = 0; i < seqIndex;) {
    byte answer = sequence[i];
    if (digitalRead(RED_BTN) == LOW && digitalRead(YELLOW_BTN) != LOW &&
        digitalRead(BLUE_BTN) != LOW && digitalRead(GREEN_BTN) != LOW) {
      red();
      if (answer != RED) return false;
      i++;
    }
    if (digitalRead(RED_BTN) != LOW && digitalRead(YELLOW_BTN) == LOW &&
        digitalRead(BLUE_BTN) != LOW && digitalRead(GREEN_BTN) != LOW) {
      yellow();
      if (answer != YELLOW) return false;
      i++;
    }
    if (digitalRead(RED_BTN) != LOW && digitalRead(YELLOW_BTN) != LOW &&
        digitalRead(BLUE_BTN) == LOW && digitalRead(GREEN_BTN) != LOW) {
      blue();
      if (answer != BLUE) return false;
      i++;
    }
    if (digitalRead(RED_BTN) != LOW && digitalRead(YELLOW_BTN) != LOW &&
        digitalRead(BLUE_BTN) != LOW && digitalRead(GREEN_BTN) == LOW) {
      green();
      if (answer != GREEN) return false;
      i++;
    }
    delay(10);
  }
  return true;
}

void setup() {
  #ifndef NDEBUG
  Serial.begin(9600);
  while (!Serial);
  #endif
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  pinMode(GREEN_BTN, INPUT_PULLUP);
  pinMode(BLUE_BTN, INPUT_PULLUP);
  pinMode(RED_BTN, INPUT_PULLUP);
  pinMode(YELLOW_BTN, INPUT_PULLUP);
  
  randomSeed(analogRead(A5) * EEPROM[0]);
  nextColor();
  delay(2000);
}

void loop() {
  showPattern();
  bool correct = checkInput();
  if (!correct) {
  	delay(300);
    buzz(fail);
    buzz(fail);
    buzz(fail);
    seqIndex = 0;
    delay(2000);
  }
  nextColor();
  delay(500);
}
