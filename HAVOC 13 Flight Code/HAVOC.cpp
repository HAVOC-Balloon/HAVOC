#include "HAVOC.h"


HAVOC::HAVOC()
{
  pinMode(CW, OUTPUT); 
  pinMode(CCW, OUTPUT); // Set thruster pins to output mode

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  //pinMode(led_teensy, OUTPUT);

  for(int i=0;i<5;i++){
    digitalWrite(CW, HIGH);
    delay(100);
    digitalWrite(CW, LOW);
    delay(100);  
    digitalWrite(CCW, HIGH);
    delay(100);
    digitalWrite(CCW, LOW);
    delay(100);
  }
  return;
}
HAVOC::HAVOC(int CW, int CCW, int sampleRate)
{
  this->CW = CW;    // Clockwise thruster pin
  this->CCW = CCW;  // Counter-Clockwise thruster pin
  pinMode(CW, OUTPUT); pinMode(CCW, OUTPUT); // Set thruster pins to output mode
  pinMode(led_1, OUTPUT); pinMode(led_2, OUTPUT);
  
  setSampleRate(sampleRate); // Set data recording sampleRate
  return;
}

void HAVOC::printData(unsigned long time, int state, float altitude, float oTarget, float oPIDError, float oPIDOutput, float x_orientation, float y_orientation, float z_orientation, float x_gyro, float y_gyro, float z_gyro, float pressure, float temp, float vTarget, float vPIDError, float vPIDOutput, bool CWOn, bool CCWOn, float PWMInput, double latitude, double longitude, long GPS_Altitude, int SIV, String UTCTime) // Prints data to OpenLog on Serial1
{
  if(time - lasttime >= sampleRate)
  {
    //Print Data row
    
    Serial1.print(packet_count);
    Serial1.print(",");
    Serial1.print(time);
    Serial1.print(",");
    Serial1.print(stateNames[state]);
    Serial1.print(",");
    Serial1.print(altitude);
    Serial1.print(",");
    Serial1.print((oTarget - 45.0));
    Serial1.print(",");
    Serial1.print(oPIDError);
    Serial1.print(",");
    Serial1.print(oPIDOutput);
    Serial1.print(",");
    Serial1.print(x_orientation);
    Serial1.print(",");
    Serial1.print(y_orientation);
    Serial1.print(",");
    Serial1.print(z_orientation);
    Serial1.print(",");
    Serial1.print(x_gyro);
    Serial1.print(",");
    Serial1.print(y_gyro);
    Serial1.print(",");
    Serial1.print(z_gyro);
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

    lasttime = time;
    packet_count++;
  }
}

void HAVOC::bootUp(Adafruit_BNO055 bno)
{
  
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
  calibrateSensors(bno); // Run sensor Calibration
  

  // Print Data Header for OpenLog
  Serial1.print("Packet Count, Time(milis.), State, Altitude(m), Target Orientation(deg.), Orientation Error(deg), Orientation PID Output,");
  Serial1.print("(X) Current Orientation(deg), (Y) Current Orientation(deg), (Z) Current Orientation(deg), ");
  Serial1.print("(X) Rotational Velocity(deg/s), (Y) Rotational Velocity(deg/s), (Z) Rotational Velocity(deg/s), Pressure(mb), ");
  Serial1.print("Temp(C), Target Velocity, Velocity Error, Velocity PID Output, Clockwise Solenoid, Counter Clockwise Solenoid, PWM Output, ");
  Serial1.print("Latitude, Longitude, Altitude(mm), SIV, UTC Time \n");
}

void HAVOC::calibrateSensors(Adafruit_BNO055 bno) // Calibrate IMU and Barometer
{
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
  
  /*int eeAddress = 0;
  adafruit_bno055_offsets_t calibrationData;
  sensor_t sensor;

  bno.getSensor(&sensor);
  


  eeAddress += sizeof(long);
  EEPROM.get(eeAddress, calibrationData);

  Serial.println("\n\nRestoring Calibration data to the BNO055...");
  bno.setSensorOffsets(calibrationData);

  Serial.println("Sensors Calibrated");*/
}

void HAVOC::setSampleRate(int sampleRate) // Sets data recording samplerate
{
  this->sampleRate = sampleRate;
}

void HAVOC::activate()
{
  Serial1.println("============================= BEGIN STABILIZATION ==============================");
  //analogWrite(led_green, 200);
}

void HAVOC::shutdown() // Shutdown payload stabilization
{
  analogWrite(CW, 0); analogWrite(CCW, 0); // Command both thrusters to close
  //analogWrite(led_green, 0);
  Serial1.println("============================= END STABILIZATION ==============================");
}
