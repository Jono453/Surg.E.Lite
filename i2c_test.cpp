//Ready for i2c testing

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define SLAVE_ADDRESS 0x04 

//Note may need to change the address ^

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int number = 0;
int state = 0;

// Fill the dots one after the other with a color (not used)
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); //pixel number from 0 to 23 (24 pixels in ring)
    strip.show();
    delay(wait);
  }
}

//Test for turning on one led if i2c communication
 void Light(int pixel){
  strip.setPixelColor(pixel,strip.Color(50,0,0));
  strip.show();
  delay(50);
  //strip.setPixelColor(pixel,strip.Color(0,0,0));
  //strip.show();
  //delay(50);
}

void Block(int pixel){
      colorWipe(strip.Color(255,255,0),50);
      colorWipe(strip.Color(0,0,0),50);
    //for(i=0; i<strip.numPixels(); i++) {  //maybe divide it into 8 so there are 3 parts each
       if (number == 1){
      strip.setPixelColor(pixel, strip.Color(0,0,0));
      
      //just want to use one intensity. Turn section off if there is a blockage
    }
       else{ 
      strip.setPixelColor(pixel, strip.Color(255,255,255));
    }
       
    strip.show();
    
    delay(50);
  }

// callback for received data


  void receiveData(int byteCount){
  
    while(Wire.available()) {
      number = Wire.read();
      Serial.print("data received: ");
      Serial.println(number);
      if (number == 1){
          digitalWrite(13, HIGH); // set the LED on
          
          //------Grace's Prototype Code-------//
          //Different sections of the ring light up based on region of interest
          //Receive commands from RaspPi then affect Neopixel LED display
          uint16_t i = 5; //declare chosen pixel/s id number
          uint32_t c = strip.Color(50,0,0);  
          strip.setPixelColor(i, c); //pixel number from 0 to 23 (24 pixels in ring)
          strip.show();  
          delay(100);
      } else if (number == 2){
          //test
          digitalWrite(13,LOW);
          uint16_t z = 5; //declare chosen pixel/s id number
          uint32_t q = strip.Color(0,0,0);  
          strip.setPixelColor(z, q); //pixel number from 0 to 23 (24 pixels in ring)
          strip.show();  
          delay(100);
      } else {
          digitalWrite(13,LOW);
          uint16_t e = 5; //declare chosen pixel/s id number
          uint32_t d = strip.Color(0,50,0);  
          strip.setPixelColor(e, d); //pixel number from 0 to 23 (24 pixels in ring)
          strip.show();  
          delay(100);
      }
    }
  }

// callback for sending data
void sendData(){
  Wire.write(number);
}

void setup(){
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //Slave......
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  Serial.println("Ready!");
}



//----unnecessary code below----// 

void loop() {
//delay(100);
}









//
//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  }
//  if(WheelPos < 170) {
//    WheelPos -= 85;
//    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//  WheelPos -= 170;
//  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}
