#include <Adafruit_CircuitPlayground.h>

#define MyBrightness 40
/* ===== Offsets must be bewteen 0 and 10 ===== */
#define CWoffset 0    //1: red
#define CCWoffset 0   //3: green
#define CW2offset 2   //2: white
#define CCW2offset 2  //4: blue

float StepSize;
float StepHold;
#define numLED 10
#define IDLED (numLED - 1)
byte ColorMode = 0, state2 = 1, state1 = 1;
float R = 0, G = 0, B = 0;
float r = MyBrightness, g = 0, b = 0;
uint32_t PastMillis;
bool flipEnd = true, flipEnd2 = true;
byte i = 0;

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
  if (millis() - PastMillis > 100) {
    PastMillis = millis();
    byte CWa, CWb, CCWa, CCWb, CWi = IDLED - i;;
    byte CW2a, CW2b, CCW2a, CCW2b;
    FirstColor(StepSize);
    SecondColor(StepSize);
/* ===== Clockwise Light 1 ===== */
    if (CWi < CWoffset) {
      CWa = CWi + (IDLED - CWoffset + 1);
    }
    else CWa = CWi - CWoffset;
    (CWa == IDLED ? CWb = 0 : CWb = CWa + 1);
    CircuitPlayground.strip.setPixelColor(CWa, R, G, B);
    CircuitPlayground.strip.setPixelColor(CWb, 0, 0, 0);

/* ===== Clockwise Light 2 ===== */
        if (CWi < CW2offset) {
          CW2a = CWi + (IDLED - CW2offset + 1);
        }
        else CW2a = CWi - CW2offset;
        (CW2a == IDLED ? CW2b = 0 : CW2b = CW2a + 1);
        CircuitPlayground.strip.setPixelColor(CW2a, R, G, B);
        CircuitPlayground.strip.setPixelColor(CW2b, 0, 0, 0);

/* ===== Counter Clockwise Light 1 ===== */
    if (i > IDLED - CCWoffset) {
      CCWa = i - (IDLED - CCWoffset + 1);
    }
    else CCWa = i + CCWoffset;
    (CCWa == 0 ? CCWb = IDLED : CCWb = CCWa - 1);
    CircuitPlayground.strip.setPixelColor(CCWa, r, g, b);
    CircuitPlayground.strip.setPixelColor(CCWb, 0, 0, 0);

/* ===== Counter Clockwise Light 2 ===== */
        if (i > IDLED - CCW2offset) {
          CCW2a = i - (IDLED - CCW2offset + 1);
        }
        else CCW2a = i + CCW2offset;
        (CCW2a == 0 ? CCW2b = IDLED : CCW2b = CCW2a - 1);
        CircuitPlayground.strip.setPixelColor(CCW2a, r, g, b);
        CircuitPlayground.strip.setPixelColor(CCW2b, 0, 0, 0);

/* ===== Overlapping Lights ===== */
    if (CCWb == CWa || CCW2b == CWa)
      CircuitPlayground.strip.setPixelColor(CWa, R, G, B);
    if (CCW2b == CW2a || CCWb == CW2a)
      CircuitPlayground.strip.setPixelColor(CW2a, R, G, B);
/**/
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
