#include <Adafruit_CircuitPlayground.h>
#define numPatterns 6

byte MyBrightness = 30;
float StepSize;
#define MyR 5
#define MyG 5
#define MyB 5

float StepHold;
#define numLED 10
#define ButtonA 4
#define ButtonB 5
byte ColorMode = 0, i = 0, state = 1;
float R = 0, G = 0, B = 0;
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
  StepSize = MyBrightness*0.02;
  //Serial.begin(115200);
  //while (!Serial) {}
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
        RunningLight();
        break;
      case 4:
        ConstBlend();
        break;
      case 5:
        RandomColors1();
        break;
      case 6:
        RandomColors2();
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
    color(StepSize);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    CircuitPlayground.strip.show();
    i++;
  }
}

void RunningBlendRainbow() {
  if (millis() - PastMillis > 120) {
    PastMillis = millis();
    float RainbowStep = (MyBrightness*2.5)/numLED;
    color(RainbowStep);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
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
    color(StepSize);
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
    if (i == 0) color(cbStepSize);
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
    CircuitPlayground.strip.show();
    i++;
  }
}

void RandomColors1() {
  color(StepSize);
  if (millis() - PastMillis > 80) {
    PastMillis = millis();
    byte iPast, iMid;
    if (i == 0) iPast = 8, iMid = 9;
    else if (i == 1) iPast = 9, iMid = 0;
    else iPast = i - 2, iMid = i - 1;
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

void RandomColors2() {
  float rcStepSize = StepSize/.2;
  color(rcStepSize);
  if (millis() - PastMillis > 60) {
    PastMillis = millis();
    CircuitPlayground.strip.setPixelColor(i, R, G, B);
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

void white() {
  R = MyBrightness;
  G = MyBrightness;
  B = MyBrightness;
}

void color(float Step) {
  if (state == 1) {
    (R < MyBrightness - StepHold ? R += Step : R = MyBrightness);
    (B > StepHold ? B -= Step : B = 0);
  }
  else if (state == 2) {
    (G < MyBrightness - StepHold ? G += Step : G = MyBrightness);
    (R > StepHold ? R -= Step : R = 0);
  }
  else if (state == 3) {
    (B < MyBrightness - StepHold ? B += Step : B = MyBrightness);
    (G > StepHold ? G -= Step : G = 0);
  }
  else state = 1;

  if ((R == MyBrightness && B == 0) ||
      (G == MyBrightness && R == 0) ||
      (B == MyBrightness && G == 0)) {
    (state < 3 ? state++ : state = 1);
  }
}

