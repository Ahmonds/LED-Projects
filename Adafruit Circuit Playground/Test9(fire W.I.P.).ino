#include <Adafruit_CircuitPlayground.h>

#define MyBrightness 30

float StepSize;
float StepHold;
#define numLED 10
#define IDLED (numLED - 1)
byte ColorMode = 0, state2 = 1, state1 = 1;
float R = 0, G = 0, B = 0;
float r = 0, g = 0, b = 0;
float FR1 = 0, FG1 = 0, FB1 = 0;
uint32_t PastMillis;
byte i, ii;

void setup() {
  StepSize = MyBrightness * 0.01;
  CircuitPlayground.begin();
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  PastMillis = millis();
}

void loop() {
  for (i = 0; i < numLED;) {
    Fire();
  }
}

void Fire() {
  if (millis() - PastMillis > random(5, 55)) {
    PastMillis = millis();
    ii = IDLED - 1;
    float ColorScalar = (i/(numLED/4));
    float ColorScalar2 = (i/(numLED/3));
    FR1 = random(MyBrightness / 3+ColorScalar, MyBrightness / 2+ColorScalar);
    FG1 = random(MyBrightness / 4+ColorScalar2, MyBrightness / 3+ColorScalar2);
    CircuitPlayground.strip.setPixelColor(i, FR1, FG1, FB1);
    CircuitPlayground.strip.show();
    i++;
  }
}
