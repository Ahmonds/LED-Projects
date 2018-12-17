#include <Adafruit_CircuitPlayground.h>

#define MyBrightness 40
#define CWoffset 0
#define CCWoffset 0

float StepSize;
float StepHold;
#define numLED 10
#define IDLED (numLED - 1)
byte ColorMode = 0, i = 0, state2 = 1, state1 = 1;
float R = 0, G = 0, B = 0;
float r = MyBrightness/4, g = 0, b = 0;
uint32_t PastMillis;
bool flipEnd = true;

void setup() {
  StepSize = MyBrightness * 0.01;
  CircuitPlayground.begin();
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  PastMillis = millis();
}

void loop() {
  for (i = 0; i < numLED;) {
    CounterRunningLights();
  }
}

void CounterRunningLights() {
  if (millis() - PastMillis > 80) {
    PastMillis = millis();
    byte CWa, CWb, CCWa, CCWb, CWi;

    CWi = IDLED - i;
    if (CWi < CWoffset) {
      CWa = CWi + (IDLED - CWoffset + 1);
    }
    else CWa = CWi - CWoffset;
    (CWa == IDLED ? CWb = 0 : CWb = CWa + 1);
    FirstColor(StepSize);
    CircuitPlayground.strip.setPixelColor(CWa, R, G, B);
    //CircuitPlayground.strip.setPixelColor(CWb, 0, 0, 0);
    CircuitPlayground.strip.setPixelColor(CWb, R/3, G/3, B/3);
//============================================================
    if (i > IDLED - CCWoffset) {
      CCWa = i - (IDLED - CCWoffset + 1);
    }
    else CCWa = i + CCWoffset;
    (CCWa == 0 ? CCWb = IDLED : CCWb = CCWa - 1);
    SecondColor(StepSize);
    CircuitPlayground.strip.setPixelColor(CCWa, r, g, b);
    //CircuitPlayground.strip.setPixelColor(CCWb, 0, 0, 0);
    CircuitPlayground.strip.setPixelColor(CCWb, r/3, g/3, b/3);
//============================================================
    if (CWa == CCWb) {
      CircuitPlayground.strip.setPixelColor(CWa, R, G, B);
    }
    if (CCWa == CWb) {
      CircuitPlayground.strip.setPixelColor(CCWa, r, g, b);
    }
    if (CWa == CCWa) {
      if (flipEnd) {
      CircuitPlayground.strip.setPixelColor(CWa, R, G, B);
      flipEnd = false;
      }
      else {
      CircuitPlayground.strip.setPixelColor(CCWa, r, g, b);
      flipEnd = true;
      }
    }
    i++;
    CircuitPlayground.strip.show();
  }
}

void FirstColor(float Step1) {
  if (state1 == 1) {
    (R < MyBrightness - StepHold ? R += Step1 : R = MyBrightness);
    (B > StepHold ? B -= Step1 : B = 0);
  }
  else if (state1 == 2) {
    (G < MyBrightness - StepHold ? G += Step1 : G = MyBrightness);
    (R > StepHold ? R -= Step1 : R = 0);
  }
  else if (state1 == 3) {
    (B < MyBrightness - StepHold ? B += Step1 : B = MyBrightness);
    (G > StepHold ? G -= Step1 : G = 0);
  }

  if ((R == MyBrightness && B == 0) ||
      (G == MyBrightness && R == 0) ||
      (B == MyBrightness && G == 0)) {
    (state1 < 3 ? state1++ : state1 = 1);
  }
}

void SecondColor(float Step2) {
  if (state2 == 1) {
    (r < MyBrightness - StepHold ? r += Step2 : r = MyBrightness);
    (b > StepHold ? b -= Step2 : b = 0);
  }
  else if (state2 == 2) {
    (g < MyBrightness - StepHold ? g += Step2 : g = MyBrightness);
    (r > StepHold ? r -= Step2 : r = 0);
  }
  else if (state2 == 3) {
    (b < MyBrightness - StepHold ? b += Step2 : b = MyBrightness);
    (g > StepHold ? g -= Step2 : g = 0);
  }

  if ((r == MyBrightness && b == 0) ||
      (g == MyBrightness && r == 0) ||
      (b == MyBrightness && g == 0)) {
    (state2 < 3 ? state2++ : state2 = 1);
  }
}
