#include "procedures/stabilization.h"

#include "Arduino.h"
#include "data.h"

Solenoids OutputTransform::getTransformed(float continuousOutput) {
  return Solenoids::SOLENOIDS_OFF;
}

Solenoids PFM::getTransformed(float continuousOutput) {
  static Timer PFMTimer(0);
  static Timer ClockwiseTimer(0);
  static Timer CounterClockwiseTimer(0); 

  if (PFMTimer.isComplete()) {
    // set all new
    onPercent = abs(continuousOutput);
    if (onPercent < deadzone) {
      onPercent = 0;
    } else if (onPercent > (1 - deadzone)) {
      onPercent = 1;
    }
    offPercent = 1 - onPercent;

    if (onPercent < deadzone) {
      onTime = -1;
      offTime = cycleTime;
    } else if (onPercent > (1 - deadzone)) {
      onTime = cycleTime;
      offTime = -1;
    } else if (onPercent >= offPercent) {
      onTime = (onPercent / offPercent) * cycleTime;
      offTime = cycleTime;
    } else if (offPercent > onPercent) {
      onTime = cycleTime;
      offTime = (offPercent / onPercent) * cycleTime;
    }

    if (onPercent == 0) {
      PFMTimer.reset(cycleTime);
      ClockwiseTimer.reset(0);
      CounterClockwiseTimer.reset(0);
    } else if (continuousOutput > 0) {
      PFMTimer.reset(onTime + offTime);
      ClockwiseTimer.reset(onTime);
      CounterClockwiseTimer.reset(0);
    } else if (continuousOutput < 0) {
      PFMTimer.reset(onTime + offTime);
      ClockwiseTimer.reset(0);
      CounterClockwiseTimer.reset(onTime);
    }
  }

  if (!ClockwiseTimer.isComplete()) {
    return Solenoids::CLOCKWISE;
  } else if (!CounterClockwiseTimer.isComplete()) {
    return Solenoids::COUNTERCLOCKWISE;
  } else {
    return Solenoids::SOLENOIDS_OFF;
  }

}

CascadedPID::CascadedPID(OutputTransform *transform) {
  outputTransform = transform;
}

CascadedPID::~CascadedPID(){delete outputTransform;}

Solenoids CascadedPID::getStabilization(Data &data) {
  float pidOutput;
  float targetVelocity;
  float velocityError;
  float okp = 0.5;
  switch (data.target.mode) {
    case TargetingMode::ORIENTATION:
      //static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
      static PIDMath oVelocityPID = PIDMath(0.05, 0, 0, 5);
      // Normalized error
      error = ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
      //pidOutput = oVelocityPID.getOutput(constrain(orientationPID.getOutput(error), -50, 50));
      //Separating these two out in order to be able to use target velocity elsewhere
      //For the love of god this should work
      //targetVelocity = constrain(orientationPID.getOutput(error), -50, 50);
      //targetVelocity = constrain(error * okp, 50, -50);
      targetVelocity = error * okp;

      if (abs(error) < 10){
        targetVelocity = 0;
      }
      else if (targetVelocity > 50){
        targetVelocity = 50;
      }
      else if (targetVelocity < -50){
        targetVelocity = -50;
      }

      velocityError = data.gyro.z - targetVelocity;
      pidOutput = oVelocityPID.getOutput(velocityError);

      if(abs(velocityError) > 5){
        //errorLED.setColor(colorPresets.red);
      }else if(abs(error) > 10){
        //errorLED.setColor(colorPresets.blue);
      }else{
        //errorLED.setColor(colorPresets.green);
      }
      break;
    case TargetingMode::VELOCITY:
      // Needs to be tuned
      static PIDMath velocityPID = PIDMath(0.05, 0, 0, 5); //0.01
      error = data.gyro.z - data.target.target;
      pidOutput = velocityPID.getOutput(error);

      if (abs(error) < 7) {
        //errorLED.setColor(colorPresets.green);
      } else {
        //errorLED.setColor(colorPresets.red); 
      }
      break;
    default:
      return Solenoids::SOLENOIDS_OFF;
  }
  return outputTransform->getTransformed(pidOutput);
}

PurePID::PurePID(OutputTransform *transform) { outputTransform = transform; }

PurePID::~PurePID() { delete outputTransform; }

Solenoids PurePID::getStabilization(Data &data) {
  // TODO
  float pidOutput;
  switch (data.target.mode) {
    case TargetingMode::ORIENTATION:
      // Needs to be tuned
      static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
      // Normalized error
      error =
          ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
      pidOutput = orientationPID.getOutput(error);
      break;
    case TargetingMode::VELOCITY:
      // Needs to be tuned
      static PIDMath velocityPID = PIDMath(0.05, 0, 0, 5); //0.01
      error = data.gyro.z - data.target.target;
      pidOutput = velocityPID.getOutput(error);

      if (abs(error) < 7) {
        errorLED.setColor(colorPresets.green);
      } else {
        errorLED.setColor(colorPresets.red); 
        /*errorLED.setColor({
            //(int)(abs(error) * 5),       // RED
            //0,                           // GREEN
            //255 - (int)(abs(error) * 5)  // BLUE
            errorLED.setColor(colorPresets.red);  
        });*/
      }
      break;
    default:
      return Solenoids::SOLENOIDS_OFF;
  }
  
  return outputTransform->getTransformed(pidOutput);  
}

// This is the new version updated to be logical (hopefully)
Solenoids BangBang::getStabilization(Data &data) {
  // TODO Update the deadband to pull from the Config file
  float velocityDeadband = 7;
  float orientationDeadband = 15;
  float maxVelocity = 30;
  float moveVelocity = 15;

  // Default to solenoids off
  Solenoids solenoidState = SOLENOIDS_OFF;

  if (data.target.mode == ORIENTATION) {
    error =
        ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
    // Correcting for high velocity
    if (abs(data.gyro.z) > maxVelocity) {
      if (data.gyro.z > 0) {
        solenoidState = CLOCKWISE;
      } else {
        solenoidState = COUNTERCLOCKWISE;
      }
      errorLED.setColor(colorPresets.red);
      return solenoidState;
    }

    if (error > orientationDeadband && data.gyro.z < moveVelocity) {
      errorLED.setColor(colorPresets.blue);
      solenoidState = COUNTERCLOCKWISE;
    } else if (error < -orientationDeadband && data.gyro.z > -moveVelocity) {
      errorLED.setColor(colorPresets.blue);
      solenoidState = CLOCKWISE;
    } else if (abs(error) < orientationDeadband) {
      errorLED.setColor(colorPresets.green);
      if (data.gyro.z > velocityDeadband) {
        solenoidState = CLOCKWISE;
      } else if (data.gyro.z < -velocityDeadband) {
        solenoidState = COUNTERCLOCKWISE;
      } else {
        solenoidState = SOLENOIDS_OFF;
      }
    }

    // errorLED.setColor({
    //            (int)(abs(velocityError) * 5),       // RED
    //            0,                                   // GREEN
    //            255 - (int)(abs(velocityError) * 5)  // BLUE
    //        });
  }

  // If within orientation bounds, control speed to the deadband
  if (data.target.mode == VELOCITY) {
    error = data.gyro.z - data.target.target;

    // Set LED based on velocity
    errorLED.setColor({
        (int)(abs(error) * 5),       // RED
        0,                           // GREEN
        255 - (int)(abs(error) * 5)  // BLUE
    });

    if (error > velocityDeadband) {
      solenoidState = CLOCKWISE;
    } else if (abs(error) > velocityDeadband) {
      solenoidState = COUNTERCLOCKWISE;
    } else {
      solenoidState = SOLENOIDS_OFF;
    }
  }

  return solenoidState;
}

PanPID::PanPID(OutputTransform *transform){outputTransform = transform;}

PanPID::~PanPID() { delete outputTransform;}

Solenoids PanPID::getStabilization(Data &data){
  float pidOutput;
}


// An angle from -180 to 180 degrees
Solenoids PhasePlane::getStabilization(Data &data) {
  // VLIMIT _______   ^
  //              .\  |   .
  //              . \ |   .<-PWINT
  //              .  \|   .
  //       <--------------------> ORIENTATION
  //              .   |\ <- SLOPE
  //       PWINT->.   | \ .  ____
  //              .   |  \_______+ DEAD
  //              .   v   .  ____- BAND
  //              VELOCITY

  // TODO Change these to pull from the config file
  static double slope = -1;
  static double velocityLimit = 10;
  static double deadband = 4;
  static double piecewiseInterval = abs(velocityLimit / slope);

  double velocity = data.gyro.z;
  double orientation = data.orientation.z;

  if (orientation > piecewiseInterval) {
    if (velocity < (-velocityLimit) - deadband) {
      return COUNTERCLOCKWISE;
    } else if (velocity > (-velocityLimit) + deadband) {
      return CLOCKWISE;
    }
  } else if (orientation < -piecewiseInterval) {
    if (velocity < velocityLimit - deadband) {
      return COUNTERCLOCKWISE;
    } else if (velocity > velocityLimit + deadband) {
      return CLOCKWISE;
    }
  } else {
    if (velocity > (orientation * slope) + deadband) {
      return COUNTERCLOCKWISE;
    } else if (velocity < (orientation * slope) - deadband) {
      return CLOCKWISE;
    }
  }
  return SOLENOIDS_OFF;

  // The below code has been commented out because it has not
  // been tested and may work if the above code does not work.
  // However, I am of the opinion that the above formulation is
  // much more readable and likely more mathematically accurate.
  /*
  double slangle = orientation * slope;
  
  if (velocityLimit <= slangle) {
    if (velocity >= velocityLimit + deadband) {
      return CLOCKWISE;
    } else if (velocity <= velocityLimit + deadband) {
      return COUNTERCLOCKWISE;
    }
  } else if (-velocityLimit - deadband >= slangle) {
    if (velocity <= -velocityLimit - deadband) {
      return COUNTERCLOCKWISE;
    } else if (velocity >= velocityLimit + deadband) {
      return CLOCKWISE;
    }
  } else {
    if (velocity >= slangle + deadband) {
      return CLOCKWISE;
    } else if (velocity <= slangle - deadband) {
      return COUNTERCLOCKWISE;
    }
  }
  return SOLENOIDS_OFF;
  */
}
