#include <Adafruit_CircuitPlayground.h>

#define NUM_LEDS 10
#define MyBrightness 20

float StepSize, StepHold;
byte R = 20, G = 5, B = 1, state1 = 1;
uint32_t PastMillis = 0;
void setup() {
  CircuitPlayground.begin();
  PastMillis = millis();
  StepSize = MyBrightness*0.01;
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
}

void loop() {
  meteorRain(1, random(30, 60), true);
}

void meteorRain(byte meteorSize, byte meteorTrailDecay, bool meteorRandomDecay) {
  for (byte i = 0; i < NUM_LEDS + NUM_LEDS;) {
    if (millis() - PastMillis > random(90, 110)) {
      PastMillis = millis();
      //FirstColor(StepSize*2);
      // fade brightness all LEDs one step
      for (byte j = 0; j < NUM_LEDS; j++) {
        if ( (!meteorRandomDecay) || (random(10) > 4) ) {
          fadeToBlack(j, meteorTrailDecay );
        }
      }

      // draw meteor
      for (byte j = 0; j < meteorSize; j++) {
        if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
          CircuitPlayground.strip.setPixelColor(i - j, R, G, B);
        }
      }

      CircuitPlayground.strip.show();
      i++;
    }
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
  uint32_t  oldColor = CircuitPlayground.strip.getPixelColor(ledNo);
  byte r = (oldColor & 0x00ff0000UL) >> 16;
  byte g = (oldColor & 0x0000ff00UL) >> 8;
  byte b = (oldColor & 0x000000ffUL);

  r = (r <= 8) ? 0 : (byte) r - (r * fadeValue / 250);
  g = (g <= 8) ? 0 : (byte) g - (g * fadeValue / 220);
  b = (b <= 8) ? 0 : (byte) b - (b * fadeValue / 200);

  CircuitPlayground.strip.setPixelColor(ledNo, r, g, b);
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
