#include <Arduino.h>
#include <arduinoFFT.h>
#include <FastLED.h>

#define SAMPLES 128        // Power of 2
#define MIC_IN A0
#define LED_PIN 2
#define NUM_LEDS 70
#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB 
#define BUTTON_PIN 3

double vReal[SAMPLES];
double vImag[SAMPLES];

int Intensity[7] = { };
int bandMapping[7] = {2, 4, 8, 16, 32, 48, 63};

CRGB leds[NUM_LEDS];
ArduinoFFT<double> FFT = ArduinoFFT<double>(); 

void setup() {
  pinMode(MIC_IN, INPUT);
  Serial.begin(115200);
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); 
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  getSamples();
  displayUpdate();
  FastLED.show();
}

void getSamples() {
  memset(Intensity, 0, sizeof(Intensity)); // Reset intensity values

  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(MIC_IN);
    vImag[i] = 0;
  }

  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);

  for (int band = 0; band < 7; band++) {
    int binIndex = bandMapping[band];
    double magnitude = vReal[binIndex] * (0.5 + band * 0.6);

    magnitude = constrain(magnitude, 0, 2047);
    magnitude = map(magnitude, 0, 2047, 0, 10);

    Intensity[band] = magnitude;
  }
}

void displayUpdate() {
  FastLED.clear();
  int color = 0;
  for (int band = 0; band < 7; band++) { 
    for (int row = 0; row < 10; row++) { 
      int ledIndex;
      if (band % 2 == 0) {
        ledIndex = (10 * (band + 1)) - row - 1;
      } else {
        ledIndex = (10 * band) + row;
      }

      if (row <= Intensity[band]) {
        leds[ledIndex] = CHSV(color, 255, BRIGHTNESS);
      } else {
        leds[ledIndex] = CRGB::Black;
      }
    }
    color += 255 / 10;
  }
}

