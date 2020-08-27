#include <Keypad.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; 
byte colPins[COLS] = {A0, A1, A2, A3};
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  
String password = "1234";
String mypassword;
 

int lock = 12;
 
int counter = 0; 
int attempts = 0; 
int max_attempts = 3; 
 
void setup(){
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.init();
  lcd.backlight();
  

  pinMode(lock, OUTPUT);
  
  
  digitalWrite(lock, LOW);
  
  Serial.println("enter password");
    lcd.print("Enter Password:");
}
  
void loop()
{
  
 keypadfunction();
 
}
 
void keypadfunction()
{
 char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
    counter = counter + 1; 
    lcd.setCursor(counter, 1);
    lcd.print("*");
  }
  if (key == '1')
  {
 
    mypassword = mypassword + 1;   
  }
  
    if (key == '2')
  {
 
    mypassword = mypassword + 2;  
  }
  
  if (key == '3')
  {
 
    mypassword = mypassword + 3; 
  }
  
   if (key == '4')
  {
  
    mypassword = mypassword + 4;  
  }
  
  if (key == '5')
  {
  
    mypassword = mypassword + 5;
  }
  
   if (key == '6')
  {
   
    mypassword = mypassword + 6; 
  }
  
   if (key == '7')
  {
 
    mypassword = mypassword + 7; 
  }
 
   if (key == '8')
  {
 
    mypassword = mypassword + 8; 
  }
  
  if (key == '9')
  {
 
    mypassword = mypassword + 9;
  }
             
                 if (key == '0')
  {
 
    mypassword = mypassword + 0; 
  }
  
  
        if (key == '*')
  {
    Serial.println(mypassword); 
    
if ( password == mypassword )
{
lcd.clear(); 
lcd.println("Welcome To");
lcd.setCursor(0,1);
lcd.println("Jigsaw Mission");
digitalWrite(lock, HIGH);
delay(10000); 
digitalWrite(lock,LOW);
mypassword = ""; 
counter = 0; 
lcd.clear();
lcd.setCursor(0,0); 
lcd.println("Enter password");
}
else
{
Serial.println("wrong");
digitalWrite(lock, LOW);
attempts = attempts + 1; 
if (attempts >= max_attempts )
{
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Locked Out");
  

attempts = 0; 
 
}
mypassword = ""; 
counter = 0; 
lcd.clear(); 
lcd.setCursor(0,0); 
lcd.print("Wrong Password");
delay(1000);
 
lcd.setCursor(0,1); 
lcd.print("max attempts 3");
delay(1000);
 
lcd.clear(); 
lcd.println("Enter password");
lcd.setCursor(0,1); 
}
    
  }  
  
  
}
