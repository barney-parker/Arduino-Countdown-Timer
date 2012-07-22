// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

//Initialise the RTC Object
RTC_DS1307 RTC;

//Button Var
volatile int state = 0;

//Backlight pin
int backlight = 7;

void setup(){
  //Set up Serial Output
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  
  //Configure the button interrupt
  attachInterrupt(0, buttonHandler, RISING);
  
  //set Digital pin 7 to OUTPUT
  pinMode(7, OUTPUT);

  //call the buttonHandler to switch on the LCD
  buttonHandler();
  
  //Set up Wire
  Wire.begin();
  
  //Start RTC
  RTC.begin();
  
  if(!RTC.isrunning()) {
    Serial.println("RTC Not Running");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  } else {
    Serial.println("RTC Initialized");
  }
}

void buttonHandler() {
  state = ((state == 0) ? 1 : 0);
  
  if(state == 0) {
    lcd.noDisplay();        //turn off the LCD display
    digitalWrite(7, LOW);  //turn the backlight off
  } else {
    lcd.display();          //turn on the LCD display
    digitalWrite(7, HIGH);  //turn the backlight on
  }
    
  Serial.print("Button State = ");  
  Serial.println(state);
}

void loop()
{
  //get the RTC time
  DateTime now = RTC.now();
  DateTime hol(2012, 10, 15, 8, 10, 0);
  
  unsigned long remaining = hol.unixtime() - now.unixtime();

  lcd.clear();

  lcd.setCursor(0,0)  ;
  //Display the time/date on the LCD
  //lcdTime(now);
  lcd.print("Time to Holidays:");

  lcd.setCursor(0,1);
  lcd.print(remaining / 86400L);
  lcd.print(":");
  lcd.print((remaining % 86400L) / 3600);
  lcd.print(":");
  lcd.print(((remaining % 84600L) % 3600) / 60);
  lcd.print(":");
  lcd.print(((remaining % 84600L) % 3600) % 60);
  
//  lcd.print(remaining);
//  lcd.print(":");

  //delay a bit...  
  delay(300);  
}

void lcdDate(DateTime d) {
  //Display the date on the LCD
  lcd.print(d.day(), DEC);
  lcd.print('/');
  lcd.print(d.month(), DEC);
  lcd.print('/');  
  lcd.print(d.year(), DEC);
}

void lcdTime(DateTime d) {
  //Display the time on the LCD
  lcd.print(d.hour(), DEC);
  lcd.print(':');
  lcd.print(d.minute(), DEC);
  lcd.print(':');
  lcd.print(d.second(), DEC);
}
