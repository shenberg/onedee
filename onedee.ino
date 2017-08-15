#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    64

CRGBArray<NUM_LEDS> leds;

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// constants won't change. They're used here to
// set pin numbers:
constexpr int buttonPin = 2;     // the number of the pushbutton pin


long startTime = 0; // bpm relative to here

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println("begin");

    // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  startTime = millis();
}

template<class T, size_t N>
constexpr int length(const T (&arr)[N]) {
  return N;
}
constexpr bool tempo[4] = {false, false, false, true};
constexpr int bpm = 120;

void fill_leds_with_tempo() {
  long time = (millis() - startTime);
  int beat = ((bpm * time) / 60000) % length(tempo);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (tempo[beat]) {
      leds[i] = CRGB::Red;
    } else {
      leds[i] = CRGB::Black;
    }
    beat = (beat + 1) % length(tempo);
  }
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  fill_leds_with_tempo();
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
  	//Serial.println("BUTTON!");
    leds.fill_solid(CRGB::Green);
  } else {
    //leds.fill_solid(CRGB::Blue);
  }
  
  FastLED.show();
  delay(1000 / FRAMES_PER_SECOND);
}