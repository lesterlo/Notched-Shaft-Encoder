// Notched Shaft Encoder differential Position Mode Example (NSEncoder_diffMode.ino)
// Copyright (c) 2020 Lester Lo 
// MIT License

// This Example shows the step difference between current and last query of the encoder.

//Program Parameter
#define WITH_OLED //Enable SSD1306 OLED Support

#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 9



// Program Start

#include <NSEncoder.h>

#ifdef WITH_OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif




NSEncoder enc(ENCODER_S1_PIN, ENCODER_S2_PIN, 4);

#ifdef WITH_OLED
Adafruit_SSD1306 oled(128, 64, &Wire, -1);//OLED 128x64 No Reset Pin
#endif

void setup() {
  Serial.begin(115200);

#ifdef WITH_OLED
  // OLED config
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  for(;;); // Don't proceed, loop forever
  }
  oled.clearDisplay();
  oled.display();
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

#ifdef WITH_OLED
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

#endif

//  delay(100); // Add delay to demonstrate the advantage of using interrupt
    
}
