#include <Adafruit_CircuitPlayground.h>

#define NUM_LEDS 10
byte MyBrightness = 15;

float StepSize, StepHold;
byte R = 0, G = 0, B = 0, state1 = 1;
uint32_t PastMillis = 0;
void setup() {
  CircuitPlayground.begin();
  PastMillis = millis();
  //MyBrightness = map(MyBrightness, 0, 100, 0, 128);
  StepSize = MyBrightness*0.01;
  (StepSize >= 1 ? StepHold = StepSize - 1 : StepHold = 0);
}

void loop() {
  meteorRain(1, random(64, 128), true);
}

void meteorRain(byte meteorSize, byte meteorTrailDecay, bool meteorRandomDecay) {
  for (byte i = 0; i < NUM_LEDS + NUM_LEDS;) {
    if (millis() - PastMillis > random(90, 120)) {
      PastMillis = millis();
    FirstColor(StepSize*5);
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
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = CircuitPlayground.strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 8) ? 0 : (byte) r - (r * fadeValue / 256);
  g = (g <= 8) ? 0 : (byte) g - (g * fadeValue / 256);
  b = (b <= 8) ? 0 : (byte) b - (b * fadeValue / 256);

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
