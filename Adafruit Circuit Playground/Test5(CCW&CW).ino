#include <Adafruit_CircuitPlayground.h>
#define numPatterns 1

byte MyBrightness = 30;
float StepSize = 0.4;
#define MyR 5
#define MyG 5
#define MyB 5

#define CWoffset 0
#define CCWoffset 0
float StepHold;
#define LEDs 10
#define numLED (LEDs - 1)
#define ButtonA 4
#define ButtonB 5
byte ColorMode = 0, i = 0, CCWstate = 1, CWstate = 2;
float R = 0, G = 0, B = 0;
float r = 0, g = 0, b = 0;
uint32_t PastMillis;
uint32_t MyColor;

#define dbDelay 100
//bool Switch = true;
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
  MyColor = CircuitPlayground.strip.Color(MyR, MyG, MyB);
  CircuitPlayground.begin();
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  PastMillis = millis();
  PrevBounceMillis = millis();
  pinMode(ButtonA, INPUT);
  digitalWrite(ButtonA, LOW);
  pinMode(ButtonB, INPUT);
  digitalWrite(ButtonB, LOW);
}

void loop() {
  for (i = 0; i < LEDs;) {
    Debounce();
    switch (ColorMode) {
      case 1:
        CounterRunningLights();
        break;
      default:
        ConstLight();
        break;
    }
  }
}

void CounterRunningLights() {
  if (millis() - PastMillis > 200) {
    PastMillis = millis();
    byte CWa, CWb, CCWa, CCWb, CWi;

//============================================================
    CWi = numLED - i;
    if (CWi < CWoffset) {
      CWa = numLED + CWoffset - CWi;
    }
    else CWa = CWi + CWoffset;
    (CWa == numLED ? CWb = 0 : CWb = CWa + 1);
    CWcolor(StepSize);
    CircuitPlayground.strip.setPixelColor(CWa, R, G, B);
    CircuitPlayground.strip.setPixelColor(CWb, 0, 0, 0);

//============================================================
    if (i < CCWoffset) {
      CCWa = numLED - CCWoffset + i;
    }
    else CCWa = i - CCWoffset;
    (CCWa == 0 ? CCWb = numLED - 1 : CCWb = CCWa - 1);
    CCWcolor(StepSize);
    CircuitPlayground.strip.setPixelColor(CCWa, r, g, b);
    CircuitPlayground.strip.setPixelColor(CCWb, 0, 0, 0);

//============================================================
    if (CCWa == CWa) {
      CircuitPlayground.strip.setPixelColor(CCWa, MyBrightness/3,
        MyBrightness/3, MyBrightness/3);
    }
    CircuitPlayground.strip.show();
    i++;
  }
}

void ConstLight() {
  CircuitPlayground.strip.setPixelColor(i, MyColor);
  CircuitPlayground.strip.show();
  i++;
}

void Debounce() {
  if (!ButtonState) {
    if (digitalRead(ButtonA) == ButtonState) PrevBounceMillis = millis();
    if (millis() - PrevBounceMillis > dbDelay) ButtonState = true;
  }

  if (ButtonState) {
    if (digitalRead(ButtonA) == ButtonState) PrevBounceMillis = millis();
    if (millis() - PrevBounceMillis > dbDelay) {
      ButtonState = false;
      (ColorMode < numPatterns ? ColorMode++ : ColorMode = 0);
    }
  }
}

void CWcolor(float Step) {
  if (CWstate == 1) {
    (R < MyBrightness - StepHold ? R += Step : R = MyBrightness);
    (B > StepHold ? B -= Step : B = 0);
  }
  else if (CWstate == 2) {
    (G < MyBrightness - StepHold ? G += Step : G = MyBrightness);
    (R > StepHold ? R -= Step : R = 0);
  }
  else if (CWstate == 3) {
    (B < MyBrightness - StepHold ? B += Step : B = MyBrightness);
    (G > StepHold ? G -= Step : G = 0);
  }
  else CWstate = 1;

  if ((R == MyBrightness && B == 0) ||
      (G == MyBrightness && R == 0) ||
      (B == MyBrightness && G == 0)) {
    (CWstate < 3 ? CWstate++ : CWstate = 1);
  }
}

void CCWcolor(float Step) {
  if (CCWstate == 1) {
    (r < MyBrightness - StepHold ? r += Step : r = MyBrightness);
    (b > StepHold ? b -= Step : b = 0);
  }
  else if (CCWstate == 2) {
    (g < MyBrightness - StepHold ? g += Step : g = MyBrightness);
    (r > StepHold ? r -= Step : r = 0);
  }
  else if (CCWstate == 3) {
    (b < MyBrightness - StepHold ? b += Step : b = MyBrightness);
    (g > StepHold ? g -= Step : g = 0);
  }
  else CCWstate = 1;

  if ((r == MyBrightness && b == 0) ||
      (g == MyBrightness && r == 0) ||
      (b == MyBrightness && g == 0)) {
    (CCWstate < 3 ? CCWstate++ : CCWstate = 1);
  }
}

