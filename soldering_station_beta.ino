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
int minTemp = 200;
int maxTemp = 450;
float setTemp = minTemp;

// LCD_I2C
LiquidCrystal_I2C lcd(0x38,16,2); // adress 0x38 if A0,A1,A2 are GND

// MAX6675
#define thermo_DO 10
#define thermo_CS 11
#define thermo_SCK 9
MAX6675 thermocouple(thermo_SCK, thermo_CS, thermo_DO);
float indTemp = 0;

// PID
float setpoint, input, output; //Define Variables we'll be connecting to
PID myPID(&input, &output, &setpoint, 2, 5, 1, DIRECT); //Specify the links and initial tuning parameters
//double aggKp=4, aggKi=0.2, aggKd=1; //Define the aggressive Tuning Parameters
//double consKp=1, consKi=0.05, consKd=0.25; //Define the conservative Tuning Parameters
float Kp=4.00, Ki=0.04, Kd=0.00; //Define tuning parameters
double gap = 0;
#define PWM_PIN 6
int PWMvalue = 0;

// SERIAL COMMUNICATION
unsigned long timeOfUpdate = 0;
int timeSinceUpdate = 0; // used to determine time since last transmission of temps
byte* toSent; // used in transmission of float numbers
char flag = 'X'; // flag needed for parameters exchange

// --SETUP
void setup() {
  pinMode(MODE_BUTTON_PIN,INPUT_PULLUP); // installing pullup resistor
  pinMode(POTENTIOMETER_INPUT_PIN, INPUT); // reading from potentiometer
  
  pinMode(PWM_PIN, OUTPUT); // setting PWM pin
  
  lcd.init(); // initializing the LCD
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Rywald");
  lcd.setCursor(2,1);
  lcd.print("productions");

  // tuning PID
  myPID.SetTunings(Kp, Ki, Kd);
  //initialize the variables for PID
  indTemp = thermocouple.readCelsius();
  input = indTemp;
  setTemp = (maxTemp-minTemp)*(analogRead(POTENTIOMETER_INPUT_PIN)/1023.00)+minTemp;
  setpoint = setTemp;
  myPID.SetOutputLimits(0,255); // setting PWMvalue limit
  myPID.SetMode(AUTOMATIC); //turn the PID on

  // SERIAL COMMUNICATION
  Serial.begin(9600);
  while(!Serial);
  timeOfUpdate = millis();

  delay(2500); // wait for MAX chip to stabilize & welcome screen
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
          if (mode<2) {
            mode = mode+1;
          }
          else mode = 0;  
        }
        previousModeButtonState = modeButtonState;
      }

      // potentiometer sygnal processing
      setTemp = (maxTemp-minTemp)*(analogRead(POTENTIOMETER_INPUT_PIN)/1023.00)+minTemp;
      setTemp = round(setTemp/5)*5;
      setpoint = setTemp;

      // behave differently in case of each mode setting
      if (mode == 0) { // OFF
        setpoint = 0;
        analogWrite(PWM_PIN,0);
        PWMvalue = 0;
      }
      else if (mode == 1) { // ON
        // stepoint tracking (PID)
        gap = abs(setpoint-input); //distance from setpoint
        myPID.Compute();
        analogWrite(PWM_PIN,output);
        PWMvalue = (output/255)*100;
      }
      else if (mode == 2) { // STBY
        // stepoint tracking (PID)
        setpoint = 200; // const setpoint
        gap = abs(setpoint-input); //distance from setpoint
//        if(gap < 5) {  //we're close to setpoint, use conservative tuning parameters
//           myPID.SetTunings(consKp, consKi, consKd);
//        }
//        else {   //we're far from setpoint, use aggressive tuning parameters
//           myPID.SetTunings(aggKp, aggKi, aggKd);
//        }
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
      lcd.print(round(setTemp/5)*5,DEC);
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
      delay(150);
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

  // PARAMETERS EXCHANGE
   if(Serial.available()>0) {
    
    flag = Serial.read();
    
    if (flag == 'T') { // transfer request flag
      toSent = (byte*) &Kp;
      Serial.write(toSent,4);
      toSent = (byte*) &Ki;
      Serial.write(toSent,4);
      toSent = (byte*) &Kd;
      Serial.write(toSent,4);
      flag = 'X';
    }
    
    else if (flag == 'R') { // receive request flag
      byte buffor[4];
      float* number = (float*)buffor;

      // counting waiting time
      long int startTime = millis();

      // waiting till all bytes come or waiting time is more than 1s
      while(Serial.available() < 12 && (millis() - startTime) < 1000) {}

      if(Serial.available() >= 12) {
        // reading Kp
        for (int i = 0; i < 4; i++) {buffor[i] = Serial.read();}
        Kp = *number;
        // reading Ki
        for (int i = 0; i < 4; i++) {buffor[i] = Serial.read();}
        Ki = *number;
        // reading Kd
        for (int i = 0; i < 4; i++) {buffor[i] = Serial.read();}
        Kd = *number;
      }

      myPID.SetTunings(Kp, Ki, Kd);
      flag = 'X';
    }
    
    else { while (Serial.available() > 0) {Serial.read();} }
  }

  // UPDATE TRANSMISSION
  timeSinceUpdate = millis() - timeOfUpdate;
  timeOfUpdate = millis();
  toSent = (byte*) &timeSinceUpdate;
  Serial.write(toSent,2);
  toSent = (byte*) &setpoint;
  Serial.write(toSent,4);
  toSent = (byte*) &indTemp;
  Serial.write(toSent,4);
  
}
