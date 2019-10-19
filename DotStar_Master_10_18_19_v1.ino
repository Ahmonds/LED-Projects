#include <Adafruit_DotStar.h>

#define DATAPIN     A6  //green
#define CLOCKPIN    A7  //yellow
#define numLED 120
#define numPatterns 8
#define ButtonB 5
/*
#define COLOR_ORDER BGR
#define FRAMES_PER_SECOND 60
#define COOLING  81
#define SPARKING 80
byte DelayTime = 25;
bool gReverseDirection = false;
CRGB leds[numLED];
*/
byte MyBrightness = 30;
float StepSize;
#define MyR 5
#define MyG 5
#define MyB 5

float StepHold;
#define IDLED (numLED - 1)
#define CWoffset 0
#define CCWoffset 0
byte ColorMode = 0, i = 0, state1 = 1, state2 = 1;
float R = 0, G = 0, B = 0;
float r = 0, g = 0, b = 0;
uint32_t PastMillis;
uint32_t MyColor;
uint16_t W1 = 10, W2 = 10, W3 = 30;

#define dbDelay 50
bool ButtonState = false;
uint32_t PrevBounceMillis;

Adafruit_DotStar strip = Adafruit_DotStar(numLED, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

void setup() {
  delay(1000);
  StepSize = MyBrightness*0.02;
//  FastLED.addLeds<DOTSTAR, DataPin, ClockPin, COLOR_ORDER>(leds, numLED).setCorrection( TypicalLEDStrip );
//  FastLED.setBrightness( BRIGHTNESS );
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
      case 8:
        MeteorRain(4, 64, true);
        //Flame();
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
    strip.setPixelColor(i, R, G, B);
    strip.show();
    i++;
  }
}

void RunningBlendRainbow() {
  if (millis() - PastMillis > 110) {
    PastMillis = millis();
    float RainbowStep = (MyBrightness*2.5)/numLED;
    FirstColor(RainbowStep);
    strip.setPixelColor(i, R, G, B);
    strip.show();
    i++;
  }
}

void ColorSwirl() {
  float rcStepSize = StepSize/150;
  FirstColor(rcStepSize);
  if (millis() - PastMillis > 30) {
    PastMillis = millis();
    strip.setPixelColor(i, R, G, B);
    strip.show();
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
    strip.setPixelColor(i, (R+r)/2, g, B);
    strip.setPixelColor(iMid, R, (G+g)/2, b);
    strip.setPixelColor(iPast, r, G, (B+b)/2);
    strip.setPixelColor(iLast, 0, 0, 0);
    strip.show();
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
    strip.setPixelColor(i, R, G, B);
    byte r = R / 3, g = G / 3, b = B / 3;
    strip.setPixelColor(iMid, r, g, b);
    r = 0;
    g = 0;
    b = 0;
    strip.setPixelColor(iPast, r, g, b);
    strip.show();
    i++;
  }
}

void ConstBlend() {
  if (millis() - PastMillis > 8) {
    PastMillis = millis();
    float cbStepSize = StepSize/3;
    if (i == 0) FirstColor(cbStepSize);
    strip.setPixelColor(i, R, G, B);
    strip.show();
    i++;
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
    strip.show();
    i++;
  }
}

void MeteorRain(byte meteorSize, byte meteorTrailDecay, bool meteorRandomDecay) {
  for (byte i = 0; i < numLED + (meteorTrailDecay/meteorSize);) {
    if (millis() - PastMillis > random(50, 60)) {
      PastMillis = millis();
      for (byte j = 0; j < numLED; j++) {
        if ( (!meteorRandomDecay) || (random(11) > 8) ) {
          fadeToBlack(j, meteorTrailDecay);
        }
      }

      for (byte j = 0; j < meteorSize; j++) {
        if ( ( i - j < numLED) && (i - j >= 0) ) {
          strip.setPixelColor(i - j, W1, W2, W3);
        }
      }
      strip.show();
      i++;
    }
  }
}
void fadeToBlack(int ledNo, byte fadeValue) {
  uint32_t oldColor;
  uint8_t w1, w2, w3;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  w1 = (oldColor & 0x00ff0000UL) >> 16;
  w2 = (oldColor & 0x0000ff00UL) >> 8;
  w3 = (oldColor & 0x000000ffUL);
  
  w1 = (w1 <= (R/6)) ? w1 = 0 : (w1 * fadeValue / 256);
  w2 = (w2 <= (G/6)) ? w2 = 0 : (w2 * fadeValue / 256);
  w3 = (w3 <= (B/6)) ? w3 = 0 : (w3 * fadeValue / 256);
  strip.setPixelColor(ledNo, w1, w2, w3);
}

/* Flame: Needs to be set dependent on the void Loop's for loop
void Flame() {
  static byte heat[numLED];

  for ( byte i = 0; i < numLED; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / numLED) + 2));
  }

  for ( byte k = numLED - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  if ( random8() < SPARKING ) {
    byte y = random8(7);
    heat[y] = qadd8( heat[y], random8(140, 200) );
  }

  for ( byte j = 0; j < numLED; j++) {
    CRGB color = HeatColor( heat[j]);
    byte pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (numLED - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
*/

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
