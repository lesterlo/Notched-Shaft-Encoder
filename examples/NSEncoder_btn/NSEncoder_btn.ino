// Notched Shaft Encoder with button Example (NSEncoder_btn.ino)
// Copyright (c) 2020 Lester Lo 
// MIT License

// This Example shows position of the encoder and response to button pressing event.

//Program Parameter
#define WITH_OLED //Enable SSD1306 OLED Support

#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 9
#define ENCODER_KEY_PIN 10


// Program Start

#include <MsTimer2.h>
#include <NSEncoder.h>

#ifdef WITH_OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif


NSEncoder_btn enc(ENCODER_S1_PIN, ENCODER_S2_PIN, ENCODER_KEY_PIN, 4, true);

#ifdef WITH_OLED
Adafruit_SSD1306 oled(128, 64, &Wire, -1);//OLED 128x64 No Reset Pin
#endif

String display_str = "Open";
int enc_position = 0;

//Timer isr
void isr_mstimer2()
{
  enc.btn_task();
}
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



  // Endcoder setup

  // Set the button monitoring parameter
  // Commented code for using default interval in this example

  // enc.setBtnDebounceInterval(30);
  // enc.setBtnPressInterval(500);
  // enc.setBtnHoldInterval(1000);

  //You must setup this timer to enable button monitoring
  MsTimer2::set(enc.getBtnDebounceInterval(), isr_mstimer2); //Set 30 ms clock
  MsTimer2::start();
}

void loop() {
  
  NSEncoder_btn::BTN_STATE enc_btn;
  
  
  if(enc.get_Position(enc_position)) //If value is updated
  {
      Serial.print("Encoder Position: ");
      Serial.println(enc_position);
  }

  enc_btn = enc.get_Button();
  switch(enc_btn)
  {
    case NSEncoder_btn::OPEN:
      display_str = "Open";
    break;
    
    case NSEncoder_btn::PRESSED:
      display_str = "Clicked";
    break;

    case NSEncoder_btn::RELEASED:
      display_str = "released";
    break;
    
    case NSEncoder_btn::HELD:
      display_str = "held";
    break;
    
    case NSEncoder_btn::DOUBLE_PRESSED:
      display_str = "Double Pressed";
    break;

  }
  
  if(enc_btn != NSEncoder_btn::OPEN)
  {
    Serial.print("Btn status: ");
    Serial.println(display_str);
  }
  
#ifdef WITH_OLED

  oled.clearDisplay();
   
  oled.setTextSize(2); // Draw 2X-scale text
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.print(F("ENC: "));
  oled.print(enc_position);
  oled.println(F(""));

  oled.setTextSize(2);
  oled.setCursor(0, 20);
  oled.print(F("BTN: "));
  oled.print(display_str);
  oled.println(F(""));

  oled.display();

#endif

//  delay(100); // Add delay to demonstrate the advantage of using interrupt
}
