/*
 * Author: Holkan Luna
 * Project: Ouróboros
 * Description: Este sketch obtiene el bpm del corazón del usuario y usa ese valor para 
 * controlar la iluminación de una tira de neoPixel replicando el latido del corazón.
 * 
 * Se añadió el sensor GSR (Galvanic Skin Response) y ahora también los datos que se
 * obtienen del mismo ayudan a generar el valor rgb que ilumina los neopixeles.
 * 
 * El resultado hasta ahora es una luz blanca con poca variación en la gama cromática
 * 
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

//VARIABLES GSR
int gsrInput = A1;

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
  int interBeats = pulseSensor.getInterBeatIntervalMs();
  int pulseAmp = pulseSensor.getPulseAmplitude(PULSE_INPUT);
  int rawInputGSR = analogRead(gsrInput);
  
  delay(20);

  
  
  if(pulseSensor.sawStartOfBeat()){

    float r = bpmVal;
    float g = map(rawInputGSR, 0, 1023, 0, 255);
    float b = map(interBeats, 100, 5000, 0, 255);
    

    Serial.println((String)"----------");
    Serial.println((String)"BPM: " + r);
    Serial.println((String)"GSR: " + g);
    Serial.println((String)"IBI: " + b + " - " + interBeats );
    Serial.println((String)"----------");

    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      delay(10);  
    }
    pixels.show();
  }
  
}
