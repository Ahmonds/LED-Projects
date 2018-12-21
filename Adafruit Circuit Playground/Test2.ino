#include <Adafruit_CircuitPlayground.h>
#define MyBrightness 128

#define numLED 10
#define IDLED (numLED - 1)
uint32_t PastMillis;

void setup() {
  CircuitPlayground.begin();
  PastMillis = millis();
}

void loop() {
    Fire(70, 200);
}

void Fire(byte Cooling, byte Sparking) {
  if (millis() - PastMillis > 40) {
    static byte heat[numLED];
    int cooldown;
 
    // Step 1.  Cool down every cell a little
    for (byte i = 0; i < numLED; i++) {
      cooldown = random(((Cooling * 10) / numLED) + 2);

      if (cooldown > heat[i]) {
        heat[i] = 0;
      } else {
        heat[i] -= cooldown;
      }
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (byte k = numLED - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < Sparking ) {
      byte y = random(7);
      heat[y] += random(160, 255);
    }

    // Step 4.  Convert heat to LED colors
    for (byte j = 0; j < numLED; j++) setPixelHeatColor(j, heat[j]);

    CircuitPlayground.strip.show();
  }
}

void setPixelHeatColor (byte Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = (temperature / 255) * 127;

  // calculate ramp up from
  byte heatramp = t192 & 0x32; // 0..50
  heatramp <<= 2; // scale up to 0..200

  // figure out which third of the spectrum we're in:
  if ( t192 > MyBrightness / 3) {                    // hottest
    CircuitPlayground.strip.setPixelColor(Pixel, 255, 200, heatramp);
  } else if ( t192 > MyBrightness*2 / 3) {            // middle
    CircuitPlayground.strip.setPixelColor(Pixel, 200, heatramp, 0);
  } else {                               // coolest
    CircuitPlayground.strip.setPixelColor(Pixel, heatramp, 0, 0);
  }
}
