#ifndef HAVOC_H
#define HAVOC_H

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


class HAVOC
{
  public:
    HAVOC(); // Default constructor
    HAVOC(int CW, int CCW, int sampleRate);  //Constructor: 
    
    void printData(unsigned long time, int state, float altitude, float oTarget, float oPIDError, float oPIDOutput, float x_orientation, float y_orientation, float z_orientation, float x_gyro, float y_gyro, float z_gyro, float pressure, float temp, float vTarget, float vPIDError, float vPIDOutput, bool CWOn, bool CCWOn, float PWMInput, double latitude, double longitude, long GPS_Altitude, int SIV, String UTCTime); //Prints data to Serial1 based on sampleRate, can be called continuously from loop();  
 
    void bootUp(Adafruit_BNO055 bno); // Initializes serial data stream and sensors

    /*
     * Getter functions: Allow main code to retrieve variables from the payload
     */
    
    void setSampleRate(int sampleRate); // Sets data samplerate
    void activate(); // Activate stabilization
    void shutdown(); // Sets thrusters closed

  private:

     // *Private functions, used in some public functions ------------------------------------
  
    void calibrateSensors(Adafruit_BNO055 bno); // Calibrates IMU and barometer

  
    // *Sensors and related variables --------------------------------------------------------
    
    int CW = 15; int CCW = 14; int led_1 = 29; int led_2 = 33;
    int sampleRate = 50; //ms
    float seaLevel = 1028; // reference pressure for sealevel (mb)
    unsigned long lasttime = 0;
    unsigned long packet_count = 0;

    const String stateNames[5] {"STANDBY", "STABILIZATION", "PAN", "STABILIZATION2", "LANDED"};
};

#endif
