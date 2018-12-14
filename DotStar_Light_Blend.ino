#include <Adafruit_DotStar.h>
#include <SPI.h>
#define numPatterns 3

byte MyBrightness = 40;
float StepSize;
#define MyR 15
#define MyG 15
#define MyB 15

float StepHold;
#define numLED 120
#define ButtonB 4
byte ColorMode = 0, i = 0, state = 1;
float R = 0, G = 0, B = 0;
uint32_t PastMillis;

#define dbDelay 100
//bool Switch = true;
bool ButtonState = false;
uint32_t PrevBounceMillis;
uint32_t MyColor;

#define DATAPIN A6
#define CLOCKPIN A7
Adafruit_DotStar strip = Adafruit_DotStar(
                           numLED, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup() {
  StepSize = MyBrightness*.001;
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
        RunningBlend();
        break;
      case 2:
        RunningBlendRainbow();
        break;
      case 3:
        RunningLight();
        break;
      default:
        ConstLight();
        break;
    }
  }
  strip.show();
  color(StepSize);
}

void RunningBlend() {
    strip.setPixelColor(i, R, G, B);
    i++;
}

void RunningBlendRainbow() {
  color(StepSize);
  strip.setPixelColor(i, R, G, B);
  i++;
}

void RunningLight () {
  color(StepSize);
  Run1();
  Run2();
  strip.show();
  i++;
}

void Run1 () {
  byte i1, iPast1, iMid1;
  (i <= 0 ? i1 = i + 0 : i1 = i - 0);
  if (i == 0) iPast1 = numLED - 1, iMid1 = numLED;
  else if (i == 1) iPast1 = numLED, iMid1 = 0;
  else iPast1 = i1 - 2, iMid1 = i1 - 1;

  byte i2, iPast2, iMid2;
  (i <= 30 ? i2 = i + 90 : i2 = i - 30);
  if (i == 0) iPast2 = numLED - 1, iMid2 = numLED;
  else if (i == 1) iPast1 = numLED, iMid1 = 0;
  else iPast1 = i2 - 2, iMid1 = i2 - 1;

  strip.setPixelColor(i1, R, G, B);
  strip.setPixelColor(i2, R, G, B);
  byte r = R / 3, g = G / 3, b = B / 3;
  strip.setPixelColor(iMid1, r, g, b);
  strip.setPixelColor(iMid2, r, g, b);

  strip.setPixelColor(iPast1, 0, 0, 0);
  strip.setPixelColor(iPast2, 0, 0, 0);

}

void Run2() {
  byte i3, iPast3, iMid3;
  (i <= 60 ? i3 = i3 + 60 : i3 = i - 60);
  if (i == 0) iPast3 = numLED - 1, iMid3 = numLED;
  else if (i == 1) iPast3 = numLED, iMid3 = 0;
  else iPast3 = i3 - 2, iMid3 = i3 - 1;

  byte i4, iPast4, iMid4;
  (i <= 90 ? i4 = i4 + 90 : i4 = i - 90);
  if (i == 0) iPast4 = numLED - 1, iMid4 = numLED;
  else if (i == 1) iPast3 = numLED, iMid3 = 0;
  else iPast3 = i4 - 2, iMid3 = i4 - 1;

  strip.setPixelColor(i3, R, G, B);
  strip.setPixelColor(i4, R, G, B);
  byte r = R / 3, g = G / 3, b = B / 3;
  strip.setPixelColor(iMid3, r, g, b);
  strip.setPixelColor(iMid4, r, g, b);

  strip.setPixelColor(iPast3, 0, 0, 0);
  strip.setPixelColor(iPast4, 0, 0, 0);

}

void ConstLight() {
  strip.setPixelColor(i, MyColor);
  i++;
  strip.show();
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
