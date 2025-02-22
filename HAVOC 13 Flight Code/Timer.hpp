/**
 *              University of Alabama in Huntsville Space Hardware Club
 *          
 * Project:         HAVOC (High Altitude Visual Orientation Control)             
 *  
 * File Name:       Timer.hpp
 * File Purpose:    This file is to supply the driver file with a timer object, which
 *                  will allow the driver file to time its events correctly. 
 * 
 * Notes:           N/A
 *
 */




// Including the Arduino.h file so that it works with certain arduino specific functions
#include "Arduino.h"


// The default timer class
class Timer{
  protected:

    // When the timer starts 
    long startTime;

  public:

    Timer() {
      startTime = (long)millis();
    }
    
    // The method that returns the time
    long getTime() {
      return (long)millis() - startTime;
    }

    // To reset the start time to the current time
    void reset() {
      startTime = (long)millis();
    }

};


// Count down timer inheriting from the timer class
class CountdownTimer : public Timer {

  private:

    // The length of the timer
    long length;

  public:

    // A default constructor that doesn't start the timer yet
    CountdownTimer() {

      startTime = (long)millis();
      length = 0;

    }

    // A constructor that starts the timer immediately
    CountdownTimer(long time) {

      startTime = (long)millis();
      length = time;

    }

    // A method to restart the timer and add a new time
    void reset(long newTime) {

      // Resetting the starting time
      reset();

      // Updating the length
      length = newTime;

    }
    
    // A method to check to see if the timer is done
    bool isDone() {
      return internal.getTime() > length;
    }

    // Returns the time left (no way :O)
    long getTimeLeft() {
      long timeLeft = length - internal.getTime();
      if(timeLeft < 0)
      {
        timeLeft = -1;
      }
      return timeLeft;
    }

};


