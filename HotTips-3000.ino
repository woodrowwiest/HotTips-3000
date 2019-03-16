/*
DIY SpotWelder - HotTips-3000

Please use caution while dealing with mains voltage :)

  Parts Include:
  - Microwave transformer, secondary rewound with single turn 70mm^2 stranded copper
  - ArduinoProMini
  - OLED Display, Potentiometer, Pushbutton, LED's x2, resistor x2, some wires and perfboard
  - 5VDC 850mA switch mode USB power supply
  - 40 Ampere 24-380VAC Solid state relay (3-30VDC input)
  - Powercord, powerswitch, and enclosure

Some code my own @intentionalrobots
  Woodrow Wiest - https://www.instagram.com/intentionalrobots/
  
Much inspiration gleaned from:
  Spot Welder Timer Example
  Check out the instructable here - http://www.instructables.com/id/DIY-Spot-Welder-From-Microwave/  
  Jack Davies, http://jackdaviesdesign.com <<< >>> http://undergroundengineering.co.uk

Would love to see if you make one!
  
*/


#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 

const int potPin = 0;                     // analog pin used to connect the potentiometer
int potVal;                               // variable to read the value from the analog pin

const int triggerPin = 2;
int triggerSwitch = 0;
const int relayPin = 3;
const int redLED = 9;
const int greenLED = 8;
int stringWidth = 0;


void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);                           // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);                            // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);                            // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

  pinMode(triggerPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
  
}


void loop(void) {
  u8g.firstPage();                            // picture loop  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  //delay(50);
            
  potVal = analogRead(potPin);                // reads the value of the potentiometer (value between 0 and 1023)
  potVal = map(potVal, 0, 1023, 10, 500);     // scale it to use it get the right time (value between 10 and 500)

  triggerSwitch = digitalRead(triggerPin);

  if (triggerSwitch == HIGH){                 // Double pulse with small delay in between
        digitalWrite(relayPin, HIGH);
        lightRed();
        delay(potVal);
        
        digitalWrite(relayPin, LOW);
        lightGreen();
        delay(35);
        
        digitalWrite(relayPin, HIGH);
        lightRed();
        delay(potVal);
                
        digitalWrite(relayPin, LOW);          // !!! - CAUTION - !!!
        lightGreen();                         // Keep in mind the weld will restart if the button stays pressed.
        delay(1000);                          // Gives the welder at least a 1 second rest between welds
  }
  
}


void draw(void) {                           // graphic commands to redraw the complete screen
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 5, 10, "Fancy Woodrow's"); 
  u8g.setScale2x2();
  u8g.setPrintPos(10,20);
  u8g.print(potVal );
  u8g.drawStr( 40, 20, "ms");
  u8g.undoScale();
  u8g.drawStr( 17, 60, "HotTips-3000");      
}


void lightRed() {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
}


void lightGreen() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
}
