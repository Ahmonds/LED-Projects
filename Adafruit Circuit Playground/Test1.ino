#include <Adafruit_CircuitPlayground.h>
#define numPatterns 7

#define MyBrightness 30
#define CWoffset 2
#define CCWoffset 2
#define CW2offset 0
#define CCW2offset 0
#define MyR 5
#define MyG 5
#define MyB 5

float StepSize;
float StepHold;
#define numLED 10
#define IDLED (numLED - 1)
byte ColorMode = 0, i = 0, state1 = 1, state2 = 1;
float R = 0, G = 0, B = 0;
float r = 0, g = 0, b = 0;
uint32_t PastMillis;
uint32_t MyColor;

#define dbDelay 100
bool ButtonState = false;
uint32_t PrevBounceMillis;

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
  StepSize = MyBrightness*0.01;
  MyColor = CircuitPlayground.strip.Color(MyR, MyG, MyB);
  CircuitPlayground.begin();
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  PastMillis = millis();
  PrevBounceMillis = millis();
}

void loop() {
  for (i = 0; i < numLED;) {
    Debounce();
    switch (ColorMode) {
      case 1:
        RunningBlend();
        break;
      case 2:
        RunningBlendRainbow();
        break;
      case 3:
        ColorSwirl();
        break;
      case 4:
        RunningColors();
        break;
      case 5:
        RunningLight();
        break;
      case 6:
        ConstBlend();
        break;
      case 7:
        CounterRunningLights();
        break;
      default:
        ConstLight();
        break;
    }
  }
}

void RunningBlend() {
  if (millis() - PastMillis > 90) {
    PastMillis = millis();
    FirstColor(StepSize*2);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    CircuitPlayground.strip.show();
    i++;
  }
}

void RunningBlendRainbow() {
  if (millis() - PastMillis > 110) {
    PastMillis = millis();
    float RainbowStep = (MyBrightness*2.5)/numLED;
    FirstColor(RainbowStep);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    CircuitPlayground.strip.show();
    i++;
  }
}

void ColorSwirl() {
  float rcStepSize = StepSize/150;
  FirstColor(rcStepSize);
  if (millis() - PastMillis > 30) {
    PastMillis = millis();
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    CircuitPlayground.strip.show();
    i++;
  }
}

void RunningColors() {
  FirstColor(StepSize);
  SecondColor(StepSize);
  if (millis() - PastMillis > 90) {
    PastMillis = millis();
    byte iPast, iMid, iLast;
    if (i == 0) iLast = IDLED - 2, iPast = IDLED - 1, iMid = IDLED;
    else if (i == 1) iLast = IDLED - 1, iPast = IDLED, iMid = 0;
    else if (i == 2) iLast = IDLED, iPast = 0, iMid = 1;
    else iLast = i - 3, iPast = i - 2, iMid = i - 1;
    CircuitPlayground.strip.setPixelColor(i, (R+r)/2, g, B);
    CircuitPlayground.strip.setPixelColor(iMid, R, (G+g)/2, b);
    CircuitPlayground.strip.setPixelColor(iPast, r, G, (B+b)/2);
    CircuitPlayground.strip.setPixelColor(iLast, 0, 0, 0);
    CircuitPlayground.strip.show();
    i++;
  }
}

void RunningLight () {
  if (millis() - PastMillis > 90) {
    PastMillis = millis();
    byte iPast, iMid;
    if (i == 0) iPast = 8, iMid = 9;
    else if (i == 1) iPast = 9, iMid = 0;
    else iPast = i - 2, iMid = i - 1;
    FirstColor(StepSize);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    byte r = R / 3, g = G / 3, b = B / 3;
    CircuitPlayground.strip.setPixelColor(iMid, r, g, b);
    r = 0;
    g = 0;
    b = 0;
    CircuitPlayground.strip.setPixelColor(iPast, r, g, b);
    CircuitPlayground.strip.show();
    i++;
  }
}

void ConstBlend() {
  if (millis() - PastMillis > 8) {
    PastMillis = millis();
    float cbStepSize = StepSize/3;
    if (i == 0) FirstColor(cbStepSize);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    CircuitPlayground.strip.show();
    i++;
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

void ConstLight() {
  CircuitPlayground.strip.setPixelColor(i, MyColor);
  CircuitPlayground.strip.show();
  i++;
}

void Debounce() {
  if (!ButtonState) {
    if (CircuitPlayground.rightButton() == ButtonState) PrevBounceMillis = millis();
    if (millis() - PrevBounceMillis > dbDelay) ButtonState = true;
  }

  if (ButtonState) {
    if (CircuitPlayground.rightButton() == ButtonState) PrevBounceMillis = millis();
    if (millis() - PrevBounceMillis > dbDelay) {
      ButtonState = false;
      (ColorMode < numPatterns ? ColorMode++ : ColorMode = 0);
    }
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
