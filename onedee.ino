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

constexpr bool tempo[] = {false, false, false, true, false, true, true};
//constexpr bool tempo2[4] = {false, false, true, true};
//constexpr bool tempos[][] = {tempo1, tempo2, tempo1, tempo1, tempo2};
constexpr int bpm = 120;
constexpr int DEBOUNCE_TIME = 20;

int position = 0;

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

void button_pressed() {
  int nextPos;
  for (nextPos = (position + 1) % NUM_LEDS; 
       (long)leds[nextPos] != 0;
       nextPos = (nextPos + 1) % NUM_LEDS) {
  }
  position = nextPos;
}

long debounceEnd = 0;

bool was_button_pressed(int currentState) {
  static enum {
    WAITING_FOR_PRESS,
    DEBOUNCE,
    WAITING_FOR_LEAVE
  } logicState;

  switch(logicState) {
    case WAITING_FOR_PRESS:
      if (currentState) {
        logicState = DEBOUNCE;
        debounceEnd = millis() + DEBOUNCE_TIME;
      }
      break;
    case DEBOUNCE:
      if (millis() >= debounceEnd) {
        if (currentState) {
          logicState = WAITING_FOR_LEAVE;
          return true;
        } else {
          logicState = WAITING_FOR_PRESS;
        }
      }
      break;
    case WAITING_FOR_LEAVE:
      if (!currentState) {
        logicState = WAITING_FOR_PRESS;
      }
      break;
  }
  return false;
}

long nextPressTime = 0;

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  fill_leds_with_tempo();
  //if ((long)leds[position] == 0) {
  leds[position] += CRGB::Green;
  //}
  long time = millis();
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  /*if ((buttonState == HIGH) && (time > nextPressTime)) {
    nextPressTime = time + DEBOUNCE_TIME;
    button_pressed();
  	//Serial.println("BUTTON!");
    //leds.fill_solid(CRGB::Green);
  } else {
    //leds.fill_solid(CRGB::Blue);
  }*/
  if (was_button_pressed(buttonState)) {
    button_pressed();
  }
  
  FastLED.show();
  delay(1000 / FRAMES_PER_SECOND);
}
