// include the library code:
#include <avr/sleep.h>
#include <avr/power.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include "defines.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, 
                  LCD_ENABLE, 
                  LCD_D4, 
                  LCD_D5, 
                  LCD_D6, 
                  LCD_D7);

//Initialise the RTC Object
RTC_DS1307 RTC;

//update vars
long nextLCDUpdate = 0;
long powerOffTime = 0;

void setPowerOffTime() {
  powerOffTime = millis() + 10000;//5 sec delay
}

void setup(){
  // set up the LCD's number of columns and rows and enable (inc blacklight)
  lcd.begin(16, 2);
  LCDEnable();
  lcd.display();
  
  //make sure we're not going to time out too quickly
  setPowerOffTime();
  
  //set Digital pin 7 to OUTPUT
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  //power up RTC
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  
  //Set up Wire (for RTC)
  Wire.begin();
  
  //Start RTC
  RTC.begin();
  
  if(!RTC.isrunning()) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC Not Running");
    RTC.adjust(DateTime(__DATE__, __TIME__));
    for(;;);
  }
}

void LCDEnable() {
  lcd.display();          //turn on the LCD display
  digitalWrite(LCD_BL, HIGH);  //turn the backlight on
}

void LCDDisable() {
  lcd.noDisplay();        //turn off the LCD display
  digitalWrite(LCD_BL, LOW);   //turn the backlight off
}

void buttonISR() {
  detachInterrupt(0);   //detach interrupts
  sleep_disable();      //disable powersaving
  LCDEnable();          //enable the LCD
  setPowerOffTime();    //reset the power off time
  nextLCDUpdate = 0;    //update the LCD immediately
}

void loop()
{

  if(millis() > nextLCDUpdate) {
    updateLCD();
    nextLCDUpdate = millis() + 1000;  //set the next update time
  }
  
/*  if(millis() > powerOffTime) {
    //attach the interrupt function
    attachInterrupt(0, buttonISR, RISING);//TODO Should Be LOW - do we need to rewire? - was RISING
    
    //disable the LCD to save power
    LCDDisable();
    
    //enable power saving
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
  }*/
}

void updateLCD() {
  //get the RTC time
  DateTime now = RTC.now();
  DateTime hol(2012, 10, 15, 8, 10, 0);

  //calculate the remaining time  
  unsigned long remaining = hol.unixtime() - now.unixtime();

  //do the LCD update
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Time to Holidays:");

  lcd.setCursor(0,1);
  lcd.print(remaining / 86400L);
  lcd.print(":");
  lcd.print((remaining % 86400L) / 3600);
  lcd.print(":");
  lcd.print(((remaining % 84600L) % 3600) / 60);
  lcd.print(":");
  lcd.print(((remaining % 84600L) % 3600) % 60);
}
