// --LIBRARIES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <PID_v1.h>

// --INITIALIZATION OF VARIABLES

// mode button
#define MODE_BUTTON_PIN 12
int previousModeButtonState = 1;
int modeButtonState = 1;
int mode = 0;

// temperature potentiometer
#define POTENTIOMETER_INPUT_PIN A0
int minTemp = 25;
int maxTemp = 450;
float setTemp = minTemp;

// LCD_I2C
LiquidCrystal_I2C lcd(0x38,16,2); // adress 0x38 if A0,A1,A2 are GND

// MAX6675
#define thermo_DO 10
#define thermo_CS 11
#define thermo_SCK 9
MAX6675 thermocouple(thermo_SCK, thermo_CS, thermo_DO);
double indTemp = 0;

// PID
double setpoint, input, output; //Define Variables we'll be connecting to
PID myPID(&input, &output, &setpoint, 2, 5, 1, DIRECT); //Specify the links and initial tuning parameters
double aggKp=4, aggKi=0.2, aggKd=1; //Define the aggressive Tuning Parameters
double consKp=1, consKi=0.05, consKd=0.25; //Define the conservative Tuning Parameters
double gap = 0;
#define PWM_PIN 6
int PWMvalue = 0;

// --SETUP
void setup() {
  pinMode(MODE_BUTTON_PIN,INPUT_PULLUP); // installing pullup resistor
  pinMode(POTENTIOMETER_INPUT_PIN, INPUT); // reading from potentiometer
  
  pinMode(PWM_PIN, OUTPUT); // setting PWM pin
  
  lcd.init(); // initializing the LCD

  delay(500); // wait for MAX chip to stabilize

  //initialize the variables for PID
  indTemp = thermocouple.readCelsius();
  input = indTemp;
  setTemp = (maxTemp-minTemp)*(analogRead(POTENTIOMETER_INPUT_PIN)/1023.00)+minTemp;
  setpoint = setTemp;
  myPID.SetOutputLimits(0,255); // setting PWMvalue limit
  myPID.SetMode(AUTOMATIC); //turn the PID on
}

// --MAIN PROGRAM
void loop() {

  // reading from thermocouple
  indTemp = thermocouple.readCelsius();
  if (indTemp == indTemp) // thermocouple (soldering-iron) connected
  { 
      input = indTemp;
      
      // mode button sygnal processing
      modeButtonState = digitalRead(MODE_BUTTON_PIN);
      delay(25);
      if (previousModeButtonState != modeButtonState)
      {
        if (!modeButtonState) {
          if (mode<2) mode = mode+1;
          else mode = 0;  
        }
        previousModeButtonState = modeButtonState;
      }

      // potentiometer sygnal processing
      setTemp = (maxTemp-minTemp)*(analogRead(POTENTIOMETER_INPUT_PIN)/1023.00)+minTemp;
      setpoint = setTemp; 

      // behave differently in case of each mode setting
      if (mode == 0) { // OFF
        analogWrite(PWM_PIN,0);
        PWMvalue = 0;
      }
      else if (mode == 1) { // ON
        // stepoint tracking (PID)
        gap = abs(setpoint-input); //distance from setpoint
        if(gap < 10) {  //we're close to setpoint, use conservative tuning parameters
           myPID.SetTunings(consKp, consKi, consKd);
        }
        else {   //we're far from setpoint, use aggressive tuning parameters
         myPID.SetTunings(aggKp, aggKi, aggKd);
        }
        myPID.Compute();
        analogWrite(PWM_PIN,output);
        PWMvalue = (output/255)*100;
      }
      else if (mode == 2) { // STBY
        // stepoint tracking (PID)
        setpoint = 200; // const setpoint
        gap = abs(setpoint-input); //distance from setpoint
        if(gap < 10) {  //we're close to setpoint, use conservative tuning parameters
           myPID.SetTunings(consKp, consKi, consKd);
        }
        else {   //we're far from setpoint, use aggressive tuning parameters
           myPID.SetTunings(aggKp, aggKi, aggKd);
        }
        myPID.Compute();
        analogWrite(PWM_PIN,output);
        PWMvalue = (output/255)*100;
      }
      else {
        lcd.clear();
        lcd.home();
        lcd.print("Error!");
        lcd.setCursor(0,1);
        lcd.print("Incorrect mode");
      }
    
      // LCD update
      lcd.home();
      lcd.print("Set ");
      lcd.print((int)round(setTemp),DEC);
      lcd.print("  ");
      lcd.setCursor(7,0);
      lcd.write((char)223);
      lcd.print("C ");
      lcd.setCursor(0,1);
      lcd.print("Tmp ");
      lcd.print((int)round(indTemp),DEC);
      lcd.print("  ");
      lcd.setCursor(7,1);
      lcd.write((char)223);
      lcd.print("C ");
      lcd.setCursor(10,0);
      lcd.print("P=");
      lcd.print(PWMvalue,DEC);
      lcd.print("  ");
      lcd.setCursor(15,0);
      lcd.write('%');
      lcd.setCursor(10,1);
      switch (mode) {
        case 0: lcd.print(" OFF "); break;
        case 1: lcd.print(" ON  "); break;
        case 2: lcd.print(" STBY"); break;
        default:
          lcd.clear();
          lcd.home();
          lcd.print("Error!");
          lcd.setCursor(0,1);
          lcd.print("Incorrect mode");
          break;
      }
      delay(200);
  }
  else // thermocouple (soldering-iron) not connected
  {
      lcd.setCursor(0,0);
      lcd.print("    Connect     ");
      lcd.setCursor(0,1);
      lcd.print(" soldering-iron ");
      //delay(1000);
      //lcd.clear();
      delay(1000);
  }
  
}
