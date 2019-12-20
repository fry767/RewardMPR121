#include "Adafruit_MPR121.h"

#define REWARD_LICK_0 2
#define REWARD_LICK_1 3
#define MASTER_GO_PIN 4

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif


//Touch Sensor, code taken from adafruit exemple
Adafruit_MPR121 cap = Adafruit_MPR121();
bool touchSensorRecognized = false;
bool boardEnable = true;

uint16_t lastInputTouched;
uint16_t currentInputTouched;




void setup() {
  /* REWARD SETUP*/
  Serial.begin(9600);
  
  pinMode(REWARD_LICK_0, OUTPUT);
  digitalWrite(REWARD_LICK_0, LOW);

  pinMode(REWARD_LICK_1, OUTPUT);
  digitalWrite(REWARD_LICK_1, LOW);

  pinMode(MASTER_GO_PIN, INPUT);

  /* Touch sensor I2C init*/
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
  } else
    touchSensorRecognized = true;

}

void loop() {
  boardEnable = digitalRead(MASTER_GO_PIN);

  if (boardEnable) {
    readAndSendMPR121Value();
  }

}

void readAndSendMPR121Value(void) {
  if (touchSensorRecognized) {
    currentInputTouched = cap.touched();
    bool lick1 = 0;
    bool lick2 = 0;
    if ((currentInputTouched & _BV(0)) && !(lastInputTouched & _BV(0)) ) {
      lick1 = 1;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currentInputTouched & _BV(0)) && (lastInputTouched & _BV(0)) ) {
      lick1 = 0;
    }
    if ((currentInputTouched & _BV(1)) && !(lastInputTouched & _BV(1)) ) {
      lick2 = 1;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currentInputTouched & _BV(1)) && (lastInputTouched & _BV(1)) ) {
      lick2 = 0;
    }
      digitalWrite(REWARD_LICK_0, lick1);
      digitalWrite(REWARD_LICK_1, lick2);
    lastInputTouched = currentInputTouched;
  }

}
