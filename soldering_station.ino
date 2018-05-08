// --LIBRARIES
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --INITIALIZATION OF VARIABLES

// initialization of Bounce class
Bounce debouncer = Bounce();

// mode button
#define MODE_BUTTON_PIN 8
int modeButtonState = 0;
int mode = 0;

// temperature potentiometer
#define POTENTIOMETER_INPUT_PIN A0
int minTemp = 25;
int maxTemp = 450;
float setTemp = minTemp;

// LCD_I2C
LiquidCrystal_I2C lcd(0x38,16,2); // adress 0x38 if A0,A1,A2 are GND

// MAX6675
int indTemp = 0;

// PID
int PWMvalue = 0;

// --SETUP
void setup() {
  
  pinMode(MODE_BUTTON_PIN,INPUT_PULLUP);
  debouncer.attach(MODE_BUTTON_PIN);
  debouncer.interval(500); // interval in ms
  
  pinMode(POTENTIOMETER_INPUT_PIN, INPUT); // reading from potentiometer

  expander.begin(0x20);
  
  lcd.init(); // initializing the LCD
  // lcd.backlight(); // jeżeli podłączyć do P7 tranzystor do sterowania podświetleniem
  
}

// --MAIN PROGRAM
void loop() {
  
  // mode button sygnal processing
  debouncer.update(); // updating the Bounce instance
  modeButtonState = debouncer.read();
  if (!modeButtonState) {
    if (mode<2) mode += mode;
    else mode = 0;  
  }

  // potentiometer sygnal processing
  setTemp = (maxTemp-minTemp)*(analogRead(POTENTIOMETER_INPUT_PIN)/1023.00)+minTemp;

  // LCD update
  lcd.home;
  lcd.print("Set ");
  lcd.print(round(setTemp),DEC);
  lcd.SetCursor(0,1);
  lcd.print("Temp ");
  lcd.print(indTemp,DEC);
  lcd.SetCursor(8,0);
  lcd.print("P = ");
  lcd.print(PWMvalue,DEC);
  lcd.write('%');
  lcd.SetCursor(8,1);
  switch (mode) {
    case 0: lcd.print("OFF");
    case 1: lcd.print("ON");
    case 2: lcd.print("PAUSE");
    default: 
  }
  
}
