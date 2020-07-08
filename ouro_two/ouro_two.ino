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
const int Threshold = 850;

//VARIABLES NEO PIXEL
#define PIN 6 
#define NUMPIXELS 120
int hue = 65563 / 2;
int brightnes = 127;
int saturation = 127;

//VARIABLES GSR
int gsrInput = A1;
unsigned int gsrValue = 0;

//GENERAL VARIABLES
int generalCount = 0;

//INIT LIBS
PulseSensorPlayground pulseSensor;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  //Conf pulse sensor obj
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_BLINK);
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
  //pixels.setBrightness(50);
  pixels.clear();
  pixels.show();
  Serial.begin(9600);
}

void loop() {
  delay(20);
  rainbow(150);

  /*
  if(pulseSensor.sawStartOfBeat()){  
    float g = (gsrValue >= 255) ? 255 : gsrValue;
    float b = map(interBeats, 200, 2000, 0, 255) ;
    
    Serial.println((String)"----------");
    Serial.println((String)"BPM: " + bpmVal);
    Serial.println((String)"GSR: " + g);
    Serial.println((String)"IBI: " + b);
    Serial.println((String)"RIBI: "+interBeats);
    Serial.println((String)"----------");

    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(r, gsrValue, b));
      delay(10);  
    }
    pixels.show();
    
  }//End if saw pulse
  */

  
 
}//End Loop


void rainbow(int wait) {
  
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    int bpmVal = pulseSensor.getBeatsPerMinute();
    int interBeats = pulseSensor.getInterBeatIntervalMs();
    int pulseAmp = pulseSensor.getPulseAmplitude();
    

    if(analogRead(gsrInput) > 90){
      gsrValue = map(analogRead(gsrInput), 91, 1023, 0, 360);
    }else{
      gsrValue = 360/2;
    }
    

    /*
    Serial.println((String) "GSR: " + gsrValue);
    Serial.println((String) "BPM: " + bpmVal );
    Serial.println((String) "IBI: " + map(interBeats, 200, 2000, 0, 255));
    Serial.println((String) "AMP: " + pulseAmp);
    */
    
  
    if(pulseSensor.sawStartOfBeat() && gsrValue != 0){
      hue = abs(map(gsrValue, 0, 360, 0, 65563));
      
      brightnes = map(interBeats, 200, 2000, 0, 255);
      
      saturation = (sqrt((gsrValue*bpmVal)) == 'NAN') ? 127 : sqrt((gsrValue*bpmVal));
      

      Serial.println((String) "----------" );
      Serial.println((String) "BPM: " + bpmVal );
      Serial.println((String) "HUE: " + hue );
      Serial.println((String) "GSR: " + gsrValue );
      Serial.println((String) "BRI: " + abs(brightnes) );
      Serial.println((String) "SAT: " + saturation );
      Serial.println((String) "----------" );
    }

    

      for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV( hue, saturation, brightnes)));
      
    }

    
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
  
}
