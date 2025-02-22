#include <Servo.h>
#include <math.h>
#include <string.h>

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <bmp3.h>
#include <bmp3_defs.h>
#include <EEPROM.h>

#include <TimeLib.h>
#include <Wire.h>
#include <WireIMXRT.h>
#include <WireKinetis.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <u-blox_config_keys.h>
#include <u-blox_structs.h>
#include <SolarPosition.h> 

//#include "HAVOC.h"
#include "PID.h"
//#include "Timer.hpp"

int CW = 15; int CCW = 14; int led_1 = 29; int led_2 = 33;
int sampleRate = 50; //ms
//float seaLevel = 1028; // reference pressure for sealevel (mb)
unsigned long lasttime = 0;
unsigned long packet_count = 0;

const String stateNames[5] {"STANDBY", "STABILIZATION", "PAN", "STABILIZATION2", "LANDED"};

/*
 * ===============================================================================================================================
 * This code is for the 10th flight of HAVOC (March 28th, 2024)
 * Payload will stabilize at an altitude above 20km. 

 * Control software by Nikki Allen
 * State machine by Owen Thrasher
 * Original Framework by Jaiden Stark

 * Will test all (new) subsytems of HAVOC overhaul leading up to April 8th, 2024 Total Solar Eclipse.
 * 
 * Data recording is enabled
 * ===============================================================================================================================
 * - Owen Thrasher
 */

float seaLevel = 1013.25; 

//================================================================
bool target = true; // <---------------- CHANGE BEFORE FLIGHT
//================================================================

double target_altitude = 20000; // Activation altitude, meters

double deactivate_altitude = 1000; // Deactivation altitude, meters

enum FlightState // Numerical definition of states is a matter of style 
{ 
  STANDBY = 0,
  STABILIZATION = 1,
  PAN = 2,
  STABILIZATION2 = 3,
  LANDED = 4
};

FlightState state = STANDBY; 

int vTarget = 0; // deg/s

float oTarget = 0.0; // deg.

// Digital pins defined for Solenoid Valves
#define CW 15
#define CCW 14

#define led_1 29
#define led_2 33


//#define led_green 33
//#define led_red 29

//#define led_teensy 13

//HAVOC payload;   // HAVOC Payload class. Object used for payload subfunctions.
// Class construction automatically runs some setup commands and IMU calibration

/*for (int n = 0; n < 5; n++)
{
  digitalWrite(CW, HIGH);
  delay(100);
  digitalWrite(CW, LOW);
  delay(100);  
  digitalWrite(CCW, HIGH);
  delay(100);
  digitalWrite(CCW, LOW);
  delay(100);
}*/

Adafruit_BNO055 bno = Adafruit_BNO055(55); // BNO IMU

Adafruit_BMP3XX bm; // BMP barometer object. 

sensors_event_t event;

imu::Vector<3> orientation;
imu::Vector<3> gyro;

SFE_UBLOX_GNSS myGNSS; // GNSS GPS object

unsigned long GPSTick = 1000;
unsigned long lastTimeUpdate = 0;
unsigned long lastTime = 0;

double latitude =  34.7249;//1859331409;  // UPDATE TO POTEET, TX
double longitude = -86.6463;//0358016115;
long altitudeMSL = 0;

int SIV;
String date; 
String UTCTime; 
int Day = 28; 
int Month = 3; 
int Year = 2024;
int Hour = 20;
int Minute = 5;
int Second = 0;

time_t t = 0;

int solar_Azimuth = 0;

long GPS_Altitude = 0;

// *PID ---------------------------------------------------------------------------------------

//Finding the milliseconds of the minimum cycle for PWM
const float PWMHZ = 25.0; //changed
const float PWM_CYCLE = 1.0 / PWMHZ;
const float PWM_MILLIS = 1000.0 * PWM_CYCLE;
const float PWM_DEADZONE = 1.5 / PWMHZ; //changed
//const float PWM_DEADZONE = 0.075;

//PID Variables
float oPIDError; // added
float oLastTarget = 0.0;
float op = 0.0;
//float okp = 0.275; //changed
float okp = 1.0;
float oi = 0.0;
float oki = 0.0;
float od = 0.0;
float okd = 0.0;
float oPIDOutput;

float speedLimit = 50;

float vPIDError;
float vLastTarget = 0.0;
float vp = 0.0;
float vkp = 0.006; // changed // was 0.014
float vi = 0.0;
float vki = 0.0;
float vd = 0.0;
float vkd = 0.00;
float vPIDOutput;

bool CWOn;
bool CCWOn;
float PWMInput = 0.0;

bool pan_done = false;

Timer oPIDTimer;
Timer vPIDTimer;
Timer telem;
Timer timer;
Timer initTimer;

CountdownTimer CWCountdown;
CountdownTimer CCWCountdown;
CountdownTimer PWMCountdown;
CountdownTimer initCountdown;

float oPID(float target);
float vPID(float target);
void PWM(float percent);

unsigned int millisAtFirstBlink = millis();

void blinkLEDs(unsigned int milliseconds);
void setSolarAngle(long latitude, long longitude);

void setup() {

  Timer testTimer;
  
  pinMode(CW, OUTPUT); 
  pinMode(CCW, OUTPUT); // Set thruster pins to output mode

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  delay(3000);
  digitalWrite(led_1, 0);
  digitalWrite(led_2, 0);

  Serial1.begin(115200); // Begin Serial1 // HAVOC 1: 115200 // HAVOC 2: 9600 // <--LIES!!
  Serial1.println("hello world");

  /*initTimer.reset();
  if(initTimer.getTime() < 500){
    PWM(.25);
  }else if(initTimer.getTime() < 1000){
    PWM(.5);
  }else{
    PWM(0);
  }

  initCountdown.reset(1000);
  if(initCountdown.getTimeLeft() > 500){
    PWM(-.25);
  }else if(initCountdown.getTimeLeft() > 0){
    PWM(-.5);
  }else if(initCountdown.isDone()){
    PWM(0);
  }*/


  for (int n = 0; n < 5; n++)
  {
    digitalWrite(CW, HIGH);
    delay(100);
    digitalWrite(CW, LOW);
    delay(100);  
    digitalWrite(CCW, HIGH);
    delay(100);
    digitalWrite(CCW, LOW);
    delay(100);
  }


  //Serial.begin(9600);

  // GPS Setup ------------------------------------------


  Wire.begin();

  do {

    if (myGNSS.begin() == true) {
        Serial1.println("GPS Online \n");
       // myGNSS.setSerialRate(38400);

       delay(100);
       break;
    } 
    
    else 
    {
        //myGNSS.factoryReset();
        Serial1.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));

        while(1)
        {
        digitalWrite(led_1, HIGH);
        digitalWrite(led_2, HIGH); 
        delay(100);
        digitalWrite(led_1, LOW);
        digitalWrite(led_2, LOW);
        delay(100);
        }

        //delay(2000); //Wait a bit before trying again to limit the Serial output
    }
  } while(1);

  //myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  myGNSS.setI2COutput(COM_TYPE_UBX);
  myGNSS.setNavigationFrequency(5);

  if (myGNSS.setDynamicModel(DYN_MODEL_AIRBORNE4g) == false){
    Serial1.println(F("*** Warning: setDynamicModel failed ***"));
    while(1){
    //Led Flasing
    delay(15);
    }
  }

  else
  {
    Serial1.println(F("Dynamic platform model changed successfully!"));
  }

  myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
  
  delay(1000);
  Serial1.println(F("Waiting for GPS lock"));
  SIV = myGNSS.getSIV();
  
  while(SIV < 3){
    SIV = myGNSS.getSIV();

    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
    delay(2000);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    delay(2000);


    //delay(500);
    //led stuff
    //delay(100);
    //led stuff
  }
 
  delay(5000);
  // Get Date
  Year = (int)myGNSS.getYear();
  Month = (int)myGNSS.getMonth();
  Day = (int)myGNSS.getDay();
  date = (String)Month + "/" + (String)Day + "/" + (String)Year;

 ////////////////////////////////////////////////

// Barometer setup --------------------------------------------------
  if (!bm.begin_I2C()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1){
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, HIGH);
      delay(1000);
      digitalWrite(led_1, LOW);
      digitalWrite(led_2, LOW);
    }
  }

  bm.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bm.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bm.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  // Cycle pressure readings to set sea level
  bm.readPressure();
  delay(10);
  bm.readPressure();
  delay(10);
  bm.readPressure();
  delay(10);
  bm.readPressure();
  seaLevel = bm.pressure/100.0;
  //-------------------------------------------------------------------- 

  // IMU Setup ---------------------------------------------------------
  Serial.begin(9600);
  Serial1.println("Class Constructed");
  Serial1.println("Standby for Bootup");

  /* Initialise the IMU sensor */
  if(!bno.begin())
  {
    Serial.println(F("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
    while(1){
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, HIGH); 
      delay(400);
      digitalWrite(led_1, LOW);
      digitalWrite(led_2, LOW);
      //analogWrite(led_T, 0); 
    }
  }
  bno.setExtCrystalUse(true);
  //CALIBRATION ALERT
  
  lasttime = millis();
  Serial.println("Sensors detected, begin calibration");

  //Calibhrate Sensor

  int eeAddress = 0;
  long bnoID;
  bool foundCalib = false;

  EEPROM.get(eeAddress, bnoID);

  adafruit_bno055_offsets_t calibrationData;
  sensor_t sensor;

    /*
    *  Look for the sensor's unique ID at the beginning oF EEPROM.
    *  This isn't foolproof, but it's better than nothing.
    */
  bno.getSensor(&sensor);
  if (bnoID != sensor.sensor_id)
  {
      Serial1.println("\nNo Calibration Data for this sensor exists in EEPROM");
      delay(500);
  }
  else
  {
      Serial1.println("\nFound Calibration for this sensor in EEPROM.");
      eeAddress += sizeof(long);
      EEPROM.get(eeAddress, calibrationData);

        //displaySensorOffsets(calibrationData);

      Serial.println("\n\nRestoring Calibration data to the BNO055...");
      bno.setSensorOffsets(calibrationData);

      Serial1.println("\n\nCalibration data loaded into BNO055");
        //foundCalib = true;
  }
  
  //////////////////////////////////////////////////

  // Print Data Header for OpenLog
  Serial1.print("Packet Count, Time(milis.), State, Altitude(m), Target Orientation(deg.), Orientation Error(deg), Orientation PID Output,");
  Serial1.print("(X) Current Orientation(deg), (Y) Current Orientation(deg), (Z) Current Orientation(deg), ");
  Serial1.print("(X) Rotational Velocity(deg/s), (Y) Rotational Velocity(deg/s), (Z) Rotational Velocity(deg/s), Pressure(mb), ");
  Serial1.print("Temp(C), Target Velocity, Velocity Error, Velocity PID Output, Clockwise Solenoid, Counter Clockwise Solenoid, PWM Output, ");
  Serial1.print("Latitude, Longitude, Altitude(mm), SIV, UTC Time \n");


  // GPS Variable Setup
 Year = (int)myGNSS.getYear();
 Month = (int)myGNSS.getMonth();
 Day = (int)myGNSS.getDay();
 date = String(Month) + "/" + String(Day) + "/" + String(Year);

  //PID Setup

  orientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  gyro.toDegrees();
  oPIDTimer.reset();
  vPIDTimer.reset();
  timer.reset();
  telem.reset();

  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  delay(3000);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);

}
 

void loop() {

  // Get time
  unsigned long time = millis();

  // Set
  blinkLEDs(1000);

  oTarget = solar_Azimuth;

  // Get Barometer Readings
  float altitude = bm.readAltitude(seaLevel);
  float temp = bm.temperature; //C
  float pressure = bm.readPressure()/(float)100.0;

  // Get IMU Readings
  bno.getEvent(&event);

  orientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  // Get GPS Data
   
  if(myGNSS.checkUblox() && time - lastTimeUpdate > GPSTick){
   
    Hour = (int)myGNSS.getHour();
    Minute = (int)myGNSS.getMinute();
    Second = (int)myGNSS.getSecond();
    UTCTime = (String)Hour + ":" + (String)Minute + ":" + (String)Second;
    
    
    latitude = ((double)myGNSS.getLatitude())*pow(10,-7);
    longitude = ((double)myGNSS.getLongitude())*pow(10,-7);
    //altitudeMSL = myGNSS.getAltitudeMSL(); 1000.0
    GPS_Altitude = (myGNSS.getAltitude() / 1000.0);
    SIV = myGNSS.getSIV();
    
    setTime(Hour, Minute, Second, Day, Month, Year);  // Set time
    t = now();                                        // Convert to epoch time

    setSolarAngle(latitude, longitude);
    
    lastTimeUpdate = time;

    //Serial.print(solar_Azimuth + "\n");
  }

  // *State Machine ----------------------------------
  switch(state)
  {
    case STANDBY:

      //Wait
        
      if (state < STABILIZATION && GPS_Altitude > 20000) // HAVOC 2: 21000; HAVOC 1: 20000
      {
        state = STABILIZATION;
      }

      break;

    case STABILIZATION:

      //Stabilize

      PWM(vPID(oPID(solar_Azimuth))); 
      
      if (state < PAN && GPS_Altitude > 28000)
      {
        state = PAN;
      }

      break;

    case PAN:

      //Pan

      if(timer.getTime() < 15000){
        PWM(vPID(oPID(solar_Azimuth)));
      }else if(timer.getTime() < 30000){
        PWM(vPID(oPID(solar_Azimuth)));
      }else if(timer.getTime() < 45000){
        PWM(vPID(oPID(solar_Azimuth)));
      }else if(timer.getTime() < 60000){
        PWM(vPID(oPID(solar_Azimuth)));
      }else{
        //timer.reset();
        pan_done = true;
      }
      
      if (state < STABILIZATION2 && pan_done == true)
      {
        state = STABILIZATION2;
      }

      break;

    case STABILIZATION2:

      //Stabilize
      PWM(vPID(oPID(solar_Azimuth))); 
      
      if (state >= STABILIZATION && GPS_Altitude < 10000)
      {
        state = LANDED;
      }

      break;

    case LANDED:

      //OFF

      digitalWrite(CW, LOW);
      digitalWrite(CCW, LOW);
      delay(500);

      break;

    default:
    // statemachine is broken
    Serial1.print("State Machine is seriously broken...");

    break;
  };

  // Run print command
  //payload.printData(time, state, altitude, oTarget, oPIDError, oPIDOutput, orientation.x(), orientation.y(), orientation.z(), gyro.x(), gyro.y(), gyro.z(), pressure, temp, vTarget, vPIDError, vPIDOutput, CWOn, CCWOn, PWMInput, latitude, longitude, GPS_Altitude, SIV, UTCTime);

  //if(time - lasttime >= sampleRate)
  //{
    //Print Data row
    
  Serial1.print(packet_count);
  Serial1.print(",");
  Serial1.print(time);
  Serial1.print(",");
  Serial1.print(stateNames[state]);
  Serial1.print(",");
  Serial1.print(altitude);
  Serial1.print(",");
  Serial1.print((oTarget - 45.0)); //Normalize to front
  Serial1.print(",");
  Serial1.print(oPIDError);
  Serial1.print(",");
  Serial1.print(oPIDOutput);
  Serial1.print(",");
  Serial1.print(orientation.x());
  Serial1.print(",");
  Serial1.print(orientation.y());
  Serial1.print(",");
  Serial1.print(orientation.z());
  Serial1.print(",");
  Serial1.print(gyro.x());
  Serial1.print(",");
  Serial1.print(gyro.y());
  Serial1.print(",");
  Serial1.print(gyro.z());
  Serial1.print(",");
  Serial1.print(pressure);
  Serial1.print(",");
  Serial1.print(temp);
  Serial1.print(",");
  Serial1.print(vTarget);
  Serial1.print(",");
  Serial1.print(vPIDError);
  Serial1.print(",");
  Serial1.print(vPIDOutput);
  Serial1.print(",");
  Serial1.print(CWOn);
  Serial1.print(",");
  Serial1.print(CCWOn);
  Serial1.print(",");
  Serial1.print(PWMInput);
  Serial1.print(",");
  Serial1.print(latitude);
  Serial1.print(",");
  Serial1.print(longitude);
  Serial1.print(",");
  Serial1.print(GPS_Altitude);
  Serial1.print(",");
  Serial1.print(SIV);
  Serial1.print(",");
  Serial1.print(UTCTime);
  Serial1.print("\n");

   // lasttime = time;
   // packet_count++;
  //}
  
}; // End of loop()

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// *PID Functions:

float oPID(float target){

  //imuStuff();
  orientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); 

  float current = orientation.x();

  //Some sort of normalization to find error
  //oPIDError = target - current
  oPIDError = int(target - (360 - current) + 540) % 360 - 180;

  //If in dead zone do nothing
  if(abs(oPIDError) < 10){
    return 0;
  }

  od = (oPIDError - op) / oPIDTimer.getTime();
  oi = oi + (oPIDError * oPIDTimer.getTime());
  op = oPIDError;

  oPIDTimer.reset();

  if(target != oLastTarget){
    oi = 0;
  }
  if(oki * oi > .2){
    oi = .2 / oki;
  }else if(oki * oi < -.2){
    oi = (-1) * (.2 / oki);
  }
  oPIDOutput = (okp * op) + (oki * oi) + (okd * od);

  if(oPIDOutput > speedLimit){
    oPIDOutput = speedLimit;
  }else if(oPIDOutput < -1 * speedLimit){
    oPIDOutput = -1 * speedLimit;
  }
  oLastTarget = target;

  return (-1) * oPIDOutput;
}

float vPID(float target){

  //imuStuff();
  orientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  float current = gyro.z();
  vPIDError = (target - current);
  if(abs(vPIDError) < 7){
    return 0;
  }

  vd = (vPIDError - vp) / vPIDTimer.getTime();
  vi = vi + (vPIDError * vPIDTimer.getTime());
  vp = vPIDError;
  
  vPIDTimer.reset();

  if(target != vLastTarget){
    vi = 0;
  }

  if(vki * vi > .05){
    vi = 0;
  }else if(vki * vi < -.05){
    vi = 0;
  }
  vLastTarget = target;
  vPIDOutput = (vkp * vp) + (vki * vi) + (vkd * vd);

  return vPIDOutput;
}

void PWM(float percent){
  /*
    3 countdown timers
    PWMCountdown: The period of the PWM cycle (time on + time off)
    CWCountdown: The time that the CW solenoid should be open
    CCWCountdown: The time that the CCW solenoid should be open

    PWM_MILLIS: Time in milliseconds for minimum actuation of solenoids
    percent = onPercent, if positive set CW, if negative set CCW
    if abs(percent) > .5, PWM_MILLIS is off time
    if abs(percent) <= .5, PWM_MILLIS is on time
  */
  if(PWMCountdown.isDone()){
    PWMInput = percent;
    float onPercent = abs(percent);
    float offPercent = 1 - onPercent;
    long onTime = 0;
    long offTime = 0;
    //Set the smaller to PWM_MILLIS
    //Set the larger to PWM_MILLIS times the ratio between the larger / the smaller

    //If in deadzone, set time to PWM millis
    //Means it wont be tie up time
    if(onPercent < PWM_DEADZONE){
      onTime = -1;
      offTime = PWM_MILLIS;
    }else if(onPercent >= (1 - PWM_DEADZONE)){
      //If the onPercent is greater than the deadzone, have the thing be open for the min time then come back and check
      onTime = PWM_MILLIS;
      offTime = -1;
    }else if(onPercent >= offPercent){
      onTime = (onPercent / offPercent) * PWM_MILLIS;
      offTime = PWM_MILLIS;
    }else if(offPercent > onPercent){
      onTime = PWM_MILLIS;
      offTime = (offPercent / onPercent) * PWM_MILLIS;
    }

    if(percent > 0){
      PWMCountdown.reset(onTime + offTime);
      CWCountdown.reset(onTime);
      
    }else if(percent < 0){
      PWMCountdown.reset(onTime + offTime);
      CCWCountdown.reset(onTime);
    }else{
      PWMCountdown.reset(onTime + offTime);
      CWCountdown.reset(-1);
      CCWCountdown.reset(-1);
    }

  }
  
  if(!CWCountdown.isDone()){
    digitalWrite(CW, HIGH);
    digitalWrite(CCW, LOW);
    CWOn = true;
    CCWOn = false;
  }else if(!CCWCountdown.isDone()){
    digitalWrite(CW, LOW);
    digitalWrite(CCW, HIGH);
    CWOn = false;
    CCWOn = true;
  }else{
    digitalWrite(CW, LOW);
    digitalWrite(CCW, LOW);
    CWOn = false;
    CCWOn = false;
  }
}

void blinkLEDs(unsigned int milliseconds)
{
  if (millis() - millisAtFirstBlink > milliseconds) {
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
    millisAtFirstBlink = millis();
  } else {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
  }
}

void setSolarAngle(long latitude, long longitude) {

  SolarPosition location(latitude, longitude);
  solar_Azimuth = ((int)location.getSolarAzimuth(t)) + 45; // + 180) % 360 - 180);    // Solar Azimuth Angle
}
