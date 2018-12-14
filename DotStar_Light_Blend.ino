#include <Adafruit_DotStar.h>
#include <SPI.h>
#define numPatterns 3

#define MyBrightness 40
#define CWoffset 0
#define CCWoffset 0
#define MyR 10
#define MyG 10
#define MyB 10

float StepSize;
float StepHold;
#define numLED 120
#define IDLED (numLED - 1)
#define ButtonB 4
byte ColorMode = 0, i = 0, state2 = 1, state1 = 1;
float R = 0, G = 0, B = 0;
float r = 0, g = 0, b = 0;
uint32_t MyColor;
uint32_t PastMillis;

#define dbDelay 110
bool ButtonState = false;
uint32_t PrevBounceMillis;

#define DATAPIN A6
#define CLOCKPIN A7
Adafruit_DotStar strip = Adafruit_DotStar(
                           numLED, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup() {
  StepSize = MyBrightness*0.02;
  strip.begin();
  strip.show();
  strip.setBrightness(255);
  MyColor = strip.Color(MyR, MyG, MyB);
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  PrevBounceMillis = millis();
  PastMillis = millis();
  pinMode(ButtonB, INPUT);
  digitalWrite(ButtonB, LOW);
}

void loop() {
  for (i = 0; i < numLED;) {
    Debounce();
    switch (ColorMode) {
      case 1:
        ConstBlend();
        break;
      case 2:
        RunningBlendRainbow();
        break;
      case 3:
        CounterRunningLights();
        break;
      default:
        ConstLight();
        break;
    }
  }
}

void ConstBlend() {
  if (i == IDLED) strip.show(), FirstColor(StepSize);
  strip.setPixelColor(i, R, G, B);
  i++;
}

void RunningBlendRainbow() {
  FirstColor(StepSize);
  strip.setPixelColor(i, R, G, B);
  if (i == IDLED) strip.show();
  i++;
}

void CounterRunningLights() {
  //  if (millis() - PastMillis > 80) {
  //    PastMillis = millis();
  byte CWa, CWb, CCWa, CCWb, CWi;

  CWi = IDLED - i;
  if (CWi < CWoffset) {
    CWa = CWi + (IDLED - CWoffset + 1);
  }
  else CWa = CWi - CWoffset;
  (CWa == IDLED ? CWb = 0 : CWb = CWa + 1);
  FirstColor(StepSize);
  strip.setPixelColor(CWa, R, G, B);
  strip.setPixelColor(CWb, 0, 0, 0);

  if (i > IDLED - CCWoffset) {
    CCWa = i - (IDLED - CCWoffset + 1);
  }
  else CCWa = i + CCWoffset;
  (CCWa == 0 ? CCWb = IDLED : CCWb = CCWa - 1);
  SecondColor(StepSize);
  strip.setPixelColor(CCWa, r, g, b);
  strip.setPixelColor(CCWb, 0, 0, 0);

  if (CCWa == CWb) {
    strip.setPixelColor(CCWa, r, g, b);
  }
  if (CWa == CCWb) {
    strip.setPixelColor(CWa, R, G, B);
  }
  i++;
  //  }
  strip.show();
}

void ConstLight() {
  strip.setPixelColor(i, MyColor);
  strip.show();
  i++;
}

void Debounce() {
  if (!ButtonState) {
    if (digitalRead(ButtonB) == ButtonState) PrevBounceMillis = millis();
    if (millis() - PrevBounceMillis > dbDelay) ButtonState = true;
  }

  if (ButtonState) {
    if (digitalRead(ButtonB) == ButtonState) PrevBounceMillis = millis();
    if (millis() - PrevBounceMillis > dbDelay) {
      ButtonState = false;
      (ColorMode < numPatterns ? ColorMode++ : ColorMode = 0);
      i = 0;
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
