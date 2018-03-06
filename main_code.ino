#include <phys253.h>
#include <phys253pins.h>
#include <ServoTINAH.h>
#include <motor.h>
#include <LiquidCrystal.h>

/***********
 * STAGE FLAG
 * 0 = BEFORE IR GATE
 * 1 = BETWEEN GATE AND HILL
 * 2 = THE HILL ITSELF
 * 3 = BETWEEN HILL AND CIRCLE
 * 4 = PASSED FIRST CROSSHAIR
 * 5 = AT CROSSHAIR
 *********/
int FLAG = 1;
 

/****************
 * DIGITAL INPUTS
 ****************/

//Digital pins 4,5,6 reserved for Toren's arm shit

int HILLpin_front = 8;
int HILLpin_back  = 9;
int IRpin         = 10; //The output of the comparator from IR circuit for gate 
int extraQRD1     = 11;
int extraQRD2     = 12;


/*****************
 * ANALOG INPUTS
 *****************/

int zipline_pin   = 11; //

/*****************
 * IR VARIABLES
 *****************/

int KHZ10 = LOW;
int KHZ1  = HIGH; //Note, high also occurs as default when sensor detects nothing.
int gateFlag      = 0; //Only continue detecting gate if it hasn't passed it already.
int IR_level;


/***************************
 * HILL DETECTION VARIABLES
 ***************************/
int HILL_DETECTED = 0; 
int HILL_DONE     = 0;
int speedAdd      = 30; //speed to add when going up the hill

/****************************
 * ANIMAL GRABBING VARIABLES
 ****************************/
int animal_stopper_count = 0;

/*****************************
 * LINE FOLLOWING VARIABLES
 *****************************/

// Pins and other constants

//Pins and other constants
int rightSensor = 1;
int leftSensor = 0;
int speedAdjust = 4;
int PAdjust = 5;
int IAdjust = 6;
int DAdjust = 7;
int leftMotor = 0;
int rightMotor = 1;
int threshold = 350;
int maxI = 30;

// Other PID variables

int left;
int right;
int detectStop1;
int detectStop2;

int i = 0;
int j = 0;
int Speed = 0;
double P = 0;
double I = 0;
double D = 0;
int kP = 0;
int kD = 0;
int kI = 0;
int error = 0;
int lastError = 0;
int recError = 0;
int q = 0;
int m = 0;
int c = 0;
double con = 0.0;
int set1 = 0;
int set2 = 0;

/*********************************************
 ********************************************/

void setup() {
	#include <phys253setup.txt>

  // Digital Inputs
	pinMode(IRpin,INPUT);
  pinMode(HILLpin_front, INPUT);
  pinMode(HILLpin_back,  INPUT);

  //Analog Inputs
  pinMode(zipline_pin, INPUT);
  
  
	Serial.begin(9600);
	LCD.clear();
	LCD.home();
	LCD.print("Running main code...");
	delay(2000);
	LCD.clear();
	LCD.home();
	
}

void loop() {

   motor.speed(leftMotor,0);
   motor.speed(rightMotor,0);
      
      while( !(startbutton() )) {
  
        // Speed control
        Speed = analogRead(speedAdjust);
        // P control
        kP = analogRead(PAdjust)/10;
        // I control
        kI = analogRead(IAdjust)/10;
        // D control
        kD = analogRead(DAdjust)/10;
        
        if( i > 100 ) {
          
          LCD.clear();
          LCD.home();
          LCD.print("V = ");
          LCD.print(Speed);
          LCD.setCursor(9,0);
          LCD.print("P = ");
          LCD.print(kP);
          LCD.setCursor(0,1);
          LCD.print("I = ");
          LCD.print(kI);
          LCD.setCursor(9,1);
          LCD.print("D = ");
          LCD.print(kD);
          i = 0; 
        }
        i++;
      }
      i = 0;
      delay(3000);
    
    // Loop for motor function
    while( !(stopbutton()) ) {

  /**************************************
     Frequency Detection Gate Segment
  ***************************************/

      LCD.clear();
      LCD.print("FLAG: ");
      LCD.print(FLAG);

      if (FLAG == 0) {
        IR_level = digitalRead(IRpin);
        Serial.println(IR_level);
        while (IR_level == KHZ10) {
          motor.speed(rightMotor,0);
          motor.speed(leftMotor,0);
          delay(100);
          IR_level = digitalRead(IRpin);
          FLAG = 1; //Do not run this test again after this loop breaks
        }
      }

    /**************************************
        Hill detector
    ***************************************/
    
      if (FLAG == 1) {
        if (!digitalRead(HILLpin_front)) {
          Speed += speedAdd;
          FLAG = 2;
        }
      } 
 
      if (FLAG == 2) {
        if (digitalRead(HILLpin_back)) {
          Speed -= speedAdd;
          FLAG = 3;
         }
      }
 
    
      /***************************************
         You're at a stopper, grab the animal.
      ****************************************/
      if (FLAG == 3) {
        if (digitalRead(extraQRD1) && digitalRead(extraQRD2)) {
          FLAG = 4;
        }
      }
      
      if (FLAG == 4) {
        if (digitalRead(extraQRD1) && digitalRead(extraQRD2)) {
          motor.speed(rightMotor, 0);
          motor.speed(leftMotor, 0);
          //Grab animals, stop tape following.
          animal_stopper_count++;
          delay(5000); //Delay for 5 seconds just for testing phase
          Serial.print("\nAnimal stopper count is: ");
          Serial.print(animal_stopper_count);
        }
      }
 
      

}
