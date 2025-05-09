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
  switch (data.target.mode) {
    case TargetingMode::ORIENTATION:
      static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
      static PIDMath oVelocityPID = PIDMath(0.3, 0, 0, 5);
      // Normalized error
      error =
          ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
      pidOutput = oVelocityPID.getOutput(
          constrain(orientationPID.getOutput(error), -50, 50));
      break;
    case TargetingMode::VELOCITY:
      static PIDMath velocityPID = PIDMath(0.006, 0, 0, 7);
      error = data.gyro.z - data.target.target;
      pidOutput = velocityPID.getOutput(error);
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
      static PIDMath velocityPID = PIDMath(0.3, 0, 0, 5); //0.01
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

// I rewrote what is below because I hate how this is laid out, but I'm too
// scared to delete it completely Solenoids BangBang::getStabilization(Data
// &data) {
//   // TODO Update this to use a deadband variable
//   // Default to solenoids off
//   Solenoids solenoidState = SOLENOIDS_OFF;
//   // Normalized orientation error
//   error = ((int)((data.orientation.x - data.target.target) + 540) % 360) -
//   180;
//   // If outside orientation bounds, correct orientation
//   if (data.target.mode == ORIENTATION && abs(error) >= 15) {
//     if (error < 0 && data.gyro.z < 30) {
//       solenoidState = CLOCKWISE;
//     } else if (error > 0 && data.gyro.z > -30) {
//       solenoidState = COUNTERCLOCKWISE;
//     } else {
//       solenoidState = SOLENOIDS_OFF;
//     }
//     // Set LED based on orientation
//     errorLED.setColor({
//         (int)(abs(error) * 1.41),       // RED
//         0,                              // GREEN
//         255 - (int)(abs(error) * 1.41)  // BLUE
//     });
//   }
//   // If within orientation bounds, control speed to 5 deg/sec
//   else if (data.target.mode == VELOCITY || abs(data.gyro.z) >= 15) {
//     float velocityError = 0;
//     if (data.target.mode == VELOCITY) {
//       velocityError = data.gyro.z - data.target.target;
//       // Set LED based on velocity
//       errorLED.setColor({
//           (int)(abs(velocityError) * 5),       // RED
//           0,                                   // GREEN
//           255 - (int)(abs(velocityError) * 5)  // BLUE
//       });
//     } else {
//       errorLED.setColor(colorPresets.green);
//     }
//     if (velocityError > 15) {
//       solenoidState = COUNTERCLOCKWISE;
//     } else if (abs(velocityError) > 15) {
//       solenoidState = CLOCKWISE;
//     } else {
//       solenoidState = SOLENOIDS_OFF;
//     }
//   }
//   return solenoidState;
// }

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

  // If outside orientation bounds, correct orientation
  // if (data.target.mode == ORIENTATION) {
  //  // Normalizing orientation error
  //  // error = fmod(data.orientation.x, 180) - data.target.target - 90;
  //
  //  //  // Checking to see if it is already going in the right direction
  //  //  if (data.gyro.z < -1 &&
  //  //      data.solenoids == COUNTERCLOCKWISE) {  // && error > 0) {
  //  //    // Set LED based on velocity
  //  //    errorLED.setColor({
  //  //        255,  // RED
  //  //        0,    // GREEN
  //  //        255   // BLUE
  //  //    });
  //  //
  //  //    return SOLENOIDS_OFF;
  //  //  } else if (data.gyro.z > 1 &&
  //  //             data.solenoids == COUNTERCLOCKWISE) {  // && error < 0) {
  //  //    // Set LED based on velocity
  //  //    errorLED.setColor({
  //  //        255,  // RED
  //  //        0,    // GREEN
  //  //        255   // BLUE
  //  //    });
  //  //
  //  //    return SOLENOIDS_OFF;
  //  //  }
  //
  //  error =
  //      ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
  //
  //  if (error < -orientationDeadband && data.gyro.z > -moveVelocity) {
  //    solenoidState = CLOCKWISE;
  //  } else if (error > orientationDeadband && data.gyro.z < moveVelocity) {
  //    solenoidState = COUNTERCLOCKWISE;
  //  } else {
  //    if (data.gyro.z > velocityDeadband) {
  //      solenoidState = CLOCKWISE;
  //    } else if (abs(data.gyro.z) > velocityDeadband) {
  //      solenoidState = COUNTERCLOCKWISE;
  //    } else {
  //      solenoidState = SOLENOIDS_OFF;
  //    }
  //  }
  //
  //  // Set LED based on orientation
  //  errorLED.setColor({
  //      (int)(abs(error) * 1.41),       // RED
  //      0,                              // GREEN
  //      255 - (int)(abs(error) * 1.41)  // BLUE
  //  });
  //
  //  return solenoidState;
  //}

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