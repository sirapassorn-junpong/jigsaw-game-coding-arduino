#include <Adafruit_GFX.h>    // Core graphics library 
#include <Adafruit_TFTLCD.h> // Hardware-specific library 
#include <TouchScreen.h> 
#if defined(__SAM3X8E__) 
   #undef __FlashStringHelper::F(string_literal) 
   #define F(string_literal) string_literal 
#endif 
#define YP A1  // must be an analog pin, use "An" notation! 
#define XM A2  // must be an analog pin, use "An" notation! 
#define YM 7   // can be a digital pin 
#define XP 6   // can be a digital pin 
#define TS_MINX 150 
#define TS_MINY 120 
#define TS_MAXX 920 
#define TS_MAXY 940 
// For better pressure precision, we need to know the resistance 
// between X+ and X- Use any multimeter to read it 
// For the one we're using, its 300 ohms across the X plate 
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); 
#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
// optional 
#define LCD_RESET A4 
// Assign human-readable names to some common 16-bit color values: 
#define  BLACK   0x0000 
#define BLUE    0x001F 
#define RED     0xF800 
#define GREEN   0x07E0 
#define CYAN    0x07FF 
#define MAGENTA 0xF81F 
#define YELLOW  0xFFE0 
#define WHITE   0xFFFF 
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); 
long startTime; 
long elapsedTime; 
int fractional = 0;  
int Min = 0; 
int sec = 0; 
int state = 3; 
void setup(void) { 
 Serial.begin(9600); 
 Serial.println(F("Paint!")); 
 pinMode(A5, INPUT_PULLUP); 
 tft.reset(); 
 uint16_t identifier = tft.readID(); 
 if(identifier == 0x9325) { 
   Serial.println(F("Found ILI9325 LCD driver")); 
 } else if(identifier == 0x9328) { 
   Serial.println(F("Found ILI9328 LCD driver")); 
 } else if(identifier == 0x7575) { 
   Serial.println(F("Found HX8347G LCD driver")); 
 } else if(identifier == 0x9341) { 
   Serial.println(F("Found ILI9341 LCD driver")); 
 } else if(identifier == 0x8357) { 
   Serial.println(F("Found HX8357D LCD driver")); 
 } else { 
   Serial.print(F("Unknown LCD driver chip: ")); 
   Serial.println(identifier, HEX); 
   Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:")); 
   Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT")); 
   Serial.println(F("should appear in the library header (Adafruit_TFT.h).")); 
   Serial.println(F("If using the breakout board, it should NOT be #defined!")); 
   Serial.println(F("Also if using the breakout, double-check that all wiring")); 
   Serial.println(F("matches the tutorial.")); 
   return; 
 } 
 tft.begin(identifier); 
 tft.fillScreen(BLACK); 
 tft.setRotation(1); 
 tft.fillRect(0, 160, tft.width(), 80, CYAN); 
 tft.setCursor(15, 165); 
 tft.setTextColor(RED);  tft.setTextSize(10); 
 tft.println("START"); 
} 
#define MINPRESSURE 10 
#define MAXPRESSURE 1000 
void loop() 
{ 
 TSPoint p = ts.getPoint(); 
 // if sharing pins, you'll need to fix the directions of the touchscreen pins 
 //pinMode(XP, OUTPUT); 
 pinMode(XM, OUTPUT); 
 pinMode(YP, OUTPUT); 
 //pinMode(YM, OUTPUT); 
 // we have some minimum pressure we consider 'valid' 
 // pressure of 0 means no pressing! 
 if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { 
   /* 
   Serial.print("X = "); Serial.print(p.x); 
   Serial.print("\tY = "); Serial.print(p.y); 
   Serial.print("\tPressure = "); Serial.println(p.z); 
   */ 
   // scale from 0->1023 to tft.width 
   p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0); 
   p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0); 
   Serial.print("("); Serial.print(p.x); 
   Serial.print(", "); Serial.print(p.y); 
   Serial.println(")"); 
   if (p.x > 160 && digitalRead(A5) == 1) { 
     Serial.println("Reset"); 
     if(state == 1){ 
       state = 2; 
       tft.fillRect(0, 160, tft.width(), 80, CYAN); 
       tft.setCursor(15, 165); 
       tft.setTextColor(RED);  tft.setTextSize(10); 
       tft.println("RESET"); 
     }else if(state == 2) { 
       elapsedTime = 0; 
       state = 3; 
       tft.fillRect(0, 160, tft.width(), 80, CYAN); 
       tft.setCursor(15, 165); 
       tft.setTextColor(RED);  tft.setTextSize(10); 
       tft.println("START"); 
     }else{ 
       state = 0; 
     } 
     delay(100); 
   } 
 } 
 if(state == 0){ 
   startTime = millis(); 
   state = 1;   
   tft.fillRect(0, 160, tft.width(), 80, CYAN); 
   tft.setCursor(45, 165); 
   tft.setTextColor(RED);  tft.setTextSize(10); 
   tft.println("STOP"); 
 }else if(state == 1){ 
   elapsedTime =   millis() - startTime;  
 } 
 if(digitalRead(A5) == 0 && state != 3 && state != 2){ 
   state = 2; 
   tft.fillRect(0, 160, tft.width(), 80, CYAN); 
   tft.setCursor(15, 165); 
   tft.setTextColor(RED);  tft.setTextSize(10); 
   tft.println("RESET"); 
 } 
 tft.setCursor(5, 15); 
 tft.setTextColor(WHITE, BLACK);   
 tft.setTextSize(7); 
 Min = (int)((elapsedTime / 1000L)/60); 
 if (Min < 10) 
   tft.print("0"); 
 tft.print(Min); 
 tft.print(":"); 
 sec = (int)((elapsedTime / 1000L) % 60); 
 if (sec < 10) 
   tft.print("0"); 
 tft.print(sec); 
 tft.print(".");   
 tft.setTextSize(5); 
 fractional = (int)((elapsedTime % 1000L)/10); 
 if (fractional < 10) 
 tft.print("0"); 
 tft.println(fractional); 
} 
