/*
 * Author: Holkan Luna
 * Project: Ouróboros
 * Description: Este sketch obtiene el bpm del corazón del usuario y usa ese valor para 
 * controlar la iluminación de una tira de neoPixel replicando el latido del corazón.
*/

//INCLUDEPULSE SENSOR LIBRARY
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//INCLUDE NEOPIXEL LIBRARY
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


//VARIABLES PULSE SENSOR
const int PULSE_INPUT = A0;
const int PULSE_BLINK = 13;
const int PULSE_FADE = 5; 
const int Threshold = 550;

//VARIABLES NEO PIXEL
#define PIN 6 
#define NUMPIXELS 120

//GENERAL VARIABLES
int generalCount = 0;

//INIT LIBS
PulseSensorPlayground pulseSensor;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  //Conf pulse sensor obj
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);
  
  pulseSensor.setThreshold(Threshold);

  if(!pulseSensor.begin()){
    for(;;){
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }

  //Conf Neopixel
  pixels.begin();
  pixels.setBrightness(50);
  pixels.clear();
  pixels.show();
  Serial.begin(9600);
}

void loop() {
  int bpmVal = pulseSensor.getBeatsPerMinute();
  delay(20);

  if(pulseSensor.sawStartOfBeat()){
    Serial.println(bpmVal);
    generalCount++;
    
    if(generalCount <= 5){
      pixels.setPixelColor(generalCount, pixels.Color(bpmVal, 0, 0));
      pixels.show();  
      delay(2);
    }else{
      int i = 0;
      int j = bpmVal;
      for(i; i<=bpmVal / 2; i++){
        pixels.setPixelColor(i, pixels.Color(bpmVal, 0, 0));
        pixels.setPixelColor(j, pixels.Color(bpmVal, 0, 0));
        pixels.show();
        delay(2);
        j--;
      }
      
      for(j; j>=0; j--){
        pixels.setPixelColor(j, pixels.Color(0, bpmVal/2, bpmVal));
        pixels.setPixelColor(i, pixels.Color(0, bpmVal/2, bpmVal));
        pixels.show();
        delay(2);
        i++;
      }
    }    
  }
}
