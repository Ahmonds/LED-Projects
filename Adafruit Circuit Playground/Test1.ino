#include <Adafruit_CircuitPlayground.h>

#define Step 1
#define StepHold (Step - 1)
#define numLED 10
byte R = 0, G = 0, B = 0, state = 1;
uint32_t PastMillis;

/*
  byte r[] = {  0,  25,  50,  76, 101, 126, 151, 177, 202, 227, 255,
            255, 227, 202, 177, 151, 126, 101,  76,  50,  25,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

  byte g[] = {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,  25,  50,  76, 101, 126, 151, 177, 202, 227, 255,
            255, 227, 202, 177, 151, 126, 101,  76,  50,  25,   0};

  byte b[] = {255, 227, 202, 177, 151, 126, 101,  76,  50,  25,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,  25,  50,  76, 101, 126, 151, 177, 202, 227, 255};
*/

void setup() {
  //Serial.begin(115200);
  //while (!Serial) {}
  CircuitPlayground.begin();
  CircuitPlayground.strip.setBrightness(255);
  PastMillis = millis();
}

void loop() {
  for (byte i = 0; i < numLED; i++) {
    if (millis() - PastMillis > 1000) {
      PastMillis = millis();
      CircuitPlayground.strip.setPixelColor(i, R, G, B);
      CircuitPlayground.strip.show();
    }
  }
  color();
}

void color() {
  if (state == 1) {
    (R < 256 - StepHold ? R += Step : R = 255);
    (B > StepHold ? B -= Step : B = 0);
  }
  else if (state == 2) {
    (G < 256 - StepHold ? G += Step : G = 255);
    (R > StepHold ? R -= Step : R = 0);
  }
  else if (state == 3) {
    (B < 256 - StepHold ? B += Step : B = 255);
    (G > StepHold ? G -= Step : G = 0);
  }
  else state = 1;

  if (R == 255 || B == 255 || G == 255) {
    (state < 3 ? state++ : state = 1);
  }
}
