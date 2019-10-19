#include <Adafruit_DotStar.h>
#include <SPI.h>
#define numPatterns 4

#define MyBrightness 100
#define CWoffset 0
#define CCWoffset 30
#define CW2offset 60
#define CCW2offset 90
#define MyR 25
#define MyG 15
#define MyB 35

float StepSize;
float StepHold;
#define numLED 120
#define IDLED (numLED - 1)
#define ButtonB 4
byte ColorMode = 0, i = 0, state2 = 1, state1 = 1;
float R = (MyBrightness/2), G = 0, B = 0;
float r = (MyBrightness/2), g = 0, b = 0;
byte MeteorDelay = random(50, 70);
uint32_t MyColor;
uint32_t PastMillis;

#define dbDelay 120
bool ButtonState = false;
uint32_t PrevBounceMillis;

#define DATAPIN A6
#define CLOCKPIN A7
Adafruit_DotStar strip = Adafruit_DotStar(
                           numLED, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

void setup() {
  StepSize = MyBrightness * 0.001;
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  pinMode(ButtonB, INPUT);
  digitalWrite(ButtonB, LOW);
  strip.begin();
  strip.show();
  MyColor = strip.Color(MyR, MyG, MyB);
  PrevBounceMillis = millis();
  PastMillis = millis();
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
      case 4:
        MeteorRain(7u, 85, true);
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
  if (i == numLED / 2 || IDLED) strip.show();
  i++;
}

void CounterRunningLights() {
    byte CWa, CWb, CCWa, CCWb, CWi = IDLED - i;;
    byte CW2a, CW2b, CCW2a, CCW2b;
    FirstColor(StepSize/2);
    SecondColor(StepSize/2);
    /* ===== Clockwise Light 1 ===== */
    if (CWi < CWoffset) {
      CWa = CWi + (IDLED - CWoffset + 1);
    }
    else CWa = CWi - CWoffset;
    (CWa == IDLED ? CWb = 0 : CWb = CWa + 1);
    strip.setPixelColor(CWa, R, G, B);
    strip.setPixelColor(CWb, 0, 0, 0);

    /* ===== Clockwise Light 2 ===== */
    if (CWi < CW2offset) {
      CW2a = CWi + (IDLED - CW2offset + 1);
    }
    else CW2a = CWi - CW2offset;
    (CW2a == IDLED ? CW2b = 0 : CW2b = CW2a + 1);
    strip.setPixelColor(CW2a, R, G, B);
    strip.setPixelColor(CW2b, 0, 0, 0);

    /* ===== Counter Clockwise Light 1 ===== */
    if (i > IDLED - CCWoffset) {
      CCWa = i - (IDLED - CCWoffset + 1);
    }
    else CCWa = i + CCWoffset;
    (CCWa == 0 ? CCWb = IDLED : CCWb = CCWa - 1);
    strip.setPixelColor(CCWa, r, g, b);
    strip.setPixelColor(CCWb, 0, 0, 0);

    /* ===== Counter Clockwise Light 2 ===== */
    if (i > IDLED - CCW2offset) {
      CCW2a = i - (IDLED - CCW2offset + 1);
    }
    else CCW2a = i + CCW2offset;
    (CCW2a == 0 ? CCW2b = IDLED : CCW2b = CCW2a - 1);
    strip.setPixelColor(CCW2a, r, g, b);
    strip.setPixelColor(CCW2b, 0, 0, 0);

    /* ===== Overlapping Lights ===== */
    if (CCWb == CWa || CCW2b == CWa)
      strip.setPixelColor(CWa, R, G, B);
    if (CCW2b == CW2a || CCWb == CW2a)
      strip.setPixelColor(CW2a, R, G, B);
    /**/
    i++;
    strip.show();
}

void MeteorRain(byte meteorSize, byte meteorTrailDecay, bool meteorRandomDecay) {
  for (i = 0; i < numLED + (meteorSize);) {
    if (millis() - PastMillis > MeteorDelay) {
      MeteorDelay = random(50, 70);
      PastMillis = millis();
      for (byte j = 0; j < numLED; j++) {
        if ( (!meteorRandomDecay) || (random(11) > 8) ) {
          fadeToBlack(j, meteorTrailDecay);
        }
      }

      for (byte j = 0; j < meteorSize; j++) {
        if ( ( i - j < numLED) && (i - j >= 0) ) {
          strip.setPixelColor(i - j, MyColor);
        }
      }

      strip.show();
      i++;
    }
  }
}
void fadeToBlack(byte ledNo, byte fadeValue) {
  uint32_t oldColor = strip.getPixelColor(ledNo);
  byte MeteorR = (oldColor & 0x00ff0000UL) >> 16;
  byte MeteorG = (oldColor & 0x0000ff00UL) >> 8;
  byte MeteorB = (oldColor & 0x000000ffUL);
  
  MeteorR = (MeteorR <= (MyR/5)) ? MeteorR = 0 : (MeteorR * fadeValue / 256);
  MeteorG = (MeteorG <= (MyG/5)) ? MeteorG = 0 : (MeteorG * fadeValue / 256);
  MeteorB = (MeteorB <= (MyB/5)) ? MeteorB = 0 : (MeteorB * fadeValue / 256);
  strip.setPixelColor(ledNo, MeteorR, MeteorG, MeteorB);
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
