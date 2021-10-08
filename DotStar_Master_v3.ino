#include <Adafruit_DotStar.h>
#include <Adafruit_CircuitPlayground.h> //Use this until further notice

#define DATAPIN     A6  //green
#define CLOCKPIN    A7  //yellow
#define numLED 120
#define numPatterns 8
#define ButtonA 4
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

byte MyBrightness = 40;
float StepSize = MyBrightness*0.01, StepHold = 0;
#define MyR 5
#define MyG 5
#define MyB 5

#define IDLED (numLED - 1)
#define CWoffset 0
#define CCWoffset 0
volatile byte ColorMode = 0, iMain = 0, state1 = 1, state2 = 1;
volatile float R = MyBrightness/2, G = 0, B = 0, r = MyBrightness/2, g = 0, b = 0;
volatile uint32_t PastMillis = 0, MyColor = 0x00000000;
volatile uint16_t W1 = 10, W2 = 10, W3 = 30;

#define dbDelayB 50
#define dbDelayAshort 50
#define dbDelayAlong 2000
volatile bool ButtonStateB = false, ButtonStateA = false;
volatile uint32_t PrevBounceMillisB = 0, PrevBounceMillisA = 0;

Adafruit_DotStar StripStar = Adafruit_DotStar(numLED, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

void setup() {
  CircuitPlayground.begin(); //Use CirPlay Lib for now
  delay(100);
//  FastLED.addLeds<DOTSTAR, DataPin, ClockPin, COLOR_ORDER>(leds, numLED).setCorrection( TypicalLEDStrip );
//  FastLED.setBrightness( BRIGHTNESS );
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
  StripStar.begin();
  delay(100);
  StripStar.show();
  MyColor = StripStar.Color(MyR, MyG, MyB);
  PrevBounceMillisB = millis();
  PrevBounceMillisA = millis();
  PastMillis = millis();
}
//sei() -> set/allow interrupts
//cli() -> clear/disable interrupts
void loop() {
  for (iMain = 0; iMain < numLED;) {
    Debounce();
//    ChangeBrightness();
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
    if (ColorMode <= numPatterns) CircuitPlayground.setPixelColor(ColorMode, R, G, B);
    else CircuitPlayground.clearPixels();
  }
}

void ConstLight() {                                                               /* 0 */
  StripStar.setPixelColor(iMain, MyColor);
  
  StripStar.show();
  iMain++;
}

void RunningBlend() {                                                             /* 1 */
  if (millis() - PastMillis > 60) {
    PastMillis = millis();
    
    FirstColor(StepSize);
    StripStar.setPixelColor(iMain, R, G, B);
    
    StripStar.show();
    iMain++;
  }
}

void RunningBlendRainbow() {                                                      /* 2 */
  if (millis() - PastMillis > 110) {
    PastMillis = millis();

    float RainbowStep = MyBrightness/numLED;
    FirstColor(RainbowStep);
    StripStar.setPixelColor(iMain, R, G, B);
    
    StripStar.show();
    iMain++;
  }
}

void ColorSwirl() {                                                               /* 3 */
  if (millis() - PastMillis > 30) {
    PastMillis = millis();
    
    float rcStepSize = StepSize/100;
    FirstColor(rcStepSize);
  
    StripStar.setPixelColor(iMain, R, G, B);
    StripStar.show();
    iMain++;
  }
}

void RunningColors() {                                                            /* 4 */
  if (millis() - PastMillis > 70) {
    PastMillis = millis();
    FirstColor(StepSize);
    SecondColor(StepSize);
    
    byte iPast, iMid, iLast;
    if (iMain == 0) iLast = IDLED - 2, iPast = IDLED - 1, iMid = IDLED;
    else if (iMain == 1) iLast = IDLED - 1, iPast = IDLED, iMid = 0;
    else if (iMain == 2) iLast = IDLED, iPast = 0, iMid = 1;
    else iLast = iMain- 3, iPast = iMain- 2, iMid = iMain- 1;
    StripStar.setPixelColor(iMain, (R+r)/2, g, B);
    StripStar.setPixelColor(iMid, R, (G+g)/2, b);
    StripStar.setPixelColor(iPast, r, G, (B+b)/2);
    StripStar.setPixelColor(iLast, 0, 0, 0);
    
    StripStar.show();
    iMain++;
  }
}

void RunningLight () {                                                            /* 5 */
  if (millis() - PastMillis > 70) {
    PastMillis = millis();
    
    byte iPast, iMid;
    if (iMain == 0) iPast = numLED-2, iMid = numLED-1;
    else if (iMain == 1) iPast = numLED-1, iMid = 0;
    else iPast = iMain-2, iMid = iMain-1;
    FirstColor(StepSize);
    
    StripStar.setPixelColor(iMain, R, G, B);
    StripStar.setPixelColor(iMid, (R/3), (G/3), (B/3));
    StripStar.setPixelColor(iPast, 0, 0, 0);
    
    StripStar.show();
    iMain++;
  }
}

void ConstBlend() {                                                               /* 6 */
  if (millis() - PastMillis > 5) {
    PastMillis = millis();
    
    float cbStepSize = StepSize/2;
    if (iMain == 0) FirstColor(cbStepSize);
    StripStar.setPixelColor(iMain, R, G, B);
    
    StripStar.show();
    iMain++;
  }
}

void CounterRunningLights() {                                                     /* 7 */
  if (millis() - PastMillis > 60) {
    PastMillis = millis();
    byte CWa, CWb, CCWa, CCWb, CWi;

    CWi = IDLED - iMain;
    if (CWi < CWoffset) {
      CWa = CWi + (IDLED - CWoffset + 1);
    }
    else CWa = CWi - CWoffset;
    (CWa == IDLED ? CWb = 0 : CWb = CWa + 1);
    
    FirstColor(StepSize);
    StripStar.setPixelColor(CWa, R, G, B);
    StripStar.setPixelColor(CWb, 0, 0, 0);


    if (iMain > IDLED - CCWoffset) {
      CCWa = iMain - (IDLED - CCWoffset + 1);
    }
    else CCWa = iMain + CCWoffset;
    (CCWa == 0 ? CCWb = IDLED : CCWb = CCWa - 1);
    
    SecondColor(StepSize);
    StripStar.setPixelColor(CCWa, r, g, b);
    StripStar.setPixelColor(CCWb, 0, 0, 0);


    if (CCWa == CWb) {
      StripStar.setPixelColor(CCWa, r, g, b);
    }
    if (CWa == CCWb) {
      StripStar.setPixelColor(CWa, R, G, B);
    }
    StripStar.show();
    iMain++;
  }
}

void MeteorRain(byte meteorSize, byte meteorTrailDecay, bool meteorRandomDecay) { /* 8 */
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
          StripStar.setPixelColor(i - j, W1, W2, W3);
        }
      }
      StripStar.show();
      i++;
    }
  }
}
void fadeToBlack(int ledNo, byte fadeValue) {
  uint32_t oldColor;
  uint8_t w1, w2, w3;
  int value;

  oldColor = StripStar.getPixelColor(ledNo);
  w1 = (oldColor & 0x00ff0000UL) >> 16;
  w2 = (oldColor & 0x0000ff00UL) >> 8;
  w3 = (oldColor & 0x000000ffUL);
  
  w1 = (w1 <= (R/6)) ? w1 = 0 : (w1 * fadeValue / 256);
  w2 = (w2 <= (G/6)) ? w2 = 0 : (w2 * fadeValue / 256);
  w3 = (w3 <= (B/6)) ? w3 = 0 : (w3 * fadeValue / 256);
  StripStar.setPixelColor(ledNo, w1, w2, w3);
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

void Debounce() {
  if (!ButtonStateB) {
    if (CircuitPlayground.rightButton() == ButtonStateB) PrevBounceMillisB = millis();
    if (millis() - PrevBounceMillisB > dbDelayB) ButtonStateB = true;
  }


  if (ButtonStateB) {
    if (CircuitPlayground.rightButton() == ButtonStateB) PrevBounceMillisB = millis();
    if (millis() - PrevBounceMillisB > dbDelayB) {
      ButtonStateB = false;
      
      (ColorMode < numPatterns ? ColorMode++ : ColorMode = 0);
      for (byte clr = 0; clr < numLED; clr++) {
        StripStar.setPixelColor(clr, 0, 0, 0);
      }
      
      StripStar.show();
      iMain = 0;
    }
  }
}

/*
void ChangeBrightness() {
  if (!ButtonStateA) {
    if (CircuitPlayground.rightButton() == ButtonStateA) PrevBounceMillisA = millis();
    if (millis() - PrevBounceMillisA > dbDelayAshort) ButtonStateA = true;
    else if (millis() - PrevBounceMillisA > dbDelayAlong) 
  }
  if (ButtonStateA) {
    if (CircuitPlayground.rightButton() == ButtonStateA) PrevBounceMillisA = millis();
    if (millis() - PrevBounceMillisA > dbDelayAshort) {
      ButtonStateA = false;
      (MyBrightness < 250 ? MyBrightness+= 5 : MyBrightness = 10);
      StepSize = MyBrightness*0.02;
    }
  }
}
*/

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
//===================================================================================================
