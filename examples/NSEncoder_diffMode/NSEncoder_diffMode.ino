// Notched Shaft Encoder differential Position Mode Example (NSEncoder_diffMode.ino)
// Copyright (c) 2020 Lester Lo 
// MIT License

// This Example shows the step difference between current and last query of the encoder.

//Program Parameter
// #define DISPLAY_MODE 0 //No external display
// #define DISPLAY_MODE 1 //Enable SSD1306 OLED Support
// #define DISPLAY_MODE 2 //Enable HD44780 LCD Support


#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 3



// Program Start

#include <NSEncoder.h>

//Display support
#if defined(DISPLAY_MODE) && DISPLAY_MODE == 1
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#elif defined(DISPLAY_MODE) && DISPLAY_MODE == 2
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#endif


NSEncoder enc(ENCODER_S1_PIN, ENCODER_S2_PIN, 4);

#if defined(DISPLAY_MODE) && DISPLAY_MODE == 1
Adafruit_SSD1306 oled(128, 64, &Wire, -1);//OLED 128x64 No Reset Pin
#elif defined(DISPLAY_MODE) && DISPLAY_MODE == 2
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#endif

void setup() {
  Serial.begin(115200);

#if defined(DISPLAY_MODE) && DISPLAY_MODE == 1
  // OLED config
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  for(;;); // Don't proceed, loop forever
  }
  oled.clearDisplay();
  oled.display();
#elif defined(DISPLAY_MODE) && DISPLAY_MODE == 2
  lcd.init();
  lcd.backlight();
#endif
}


int last_diff = 0;
void loop() {
  int diff = 0;
  if((diff = enc.get_diffPosition()) != 0) //If value is updated
  {
    Serial.print("Encoder Diff: ");
    Serial.println(diff);

    last_diff = diff;
  }

#if defined(DISPLAY_MODE) && DISPLAY_MODE == 1
  oled.clearDisplay();
   
  oled.setTextSize(2); // Draw 2X-scale text
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.print(F("Diff: "));
  oled.print(diff);
  oled.println(F(""));
  oled.print(F("Last: "));
  oled.print(last_diff);
  oled.println(F(""));

  oled.display();

#elif defined(DISPLAY_MODE) && DISPLAY_MODE == 2

  lcd.setCursor(0,0);
  lcd.print("Diff: ");
  lcd.print(diff);
  lcd.print("           ");
  lcd.setCursor(0,1);
  lcd.print("Last: ");
  lcd.print(last_diff);
  lcd.print("           ");
#endif

//  delay(100); // Add delay to demonstrate the advantage of using interrupt
    
}
