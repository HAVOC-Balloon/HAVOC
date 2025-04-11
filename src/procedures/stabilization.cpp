#include "data.h"
#include "procedures/stabilization.h"

Solenoids OutputTransform::getTransformed(float continuousOutput) {
    return Solenoids::SOLENOIDS_OFF;
}

Solenoids PFM::getTransformed(float continuousOutput){
    static Timer PFMTimer(0);
    static Timer ClockwiseTimer(0);
    static Timer CounterClockwiseTimer(0);

    if(PFMTimer.isComplete()){
        //set all new
        onPercent = abs(continuousOutput);
        if(onPercent < deadzone){
            onPercent = 0;
        }else if(onPercent > (1 - deadzone)){
            onPercent = 1;
        }
        offPercent = 1 - onPercent;

        if(onPercent < deadzone){
            onTime = -1;
            offTime = cycleTime;
        }else if(onPercent > (1 - deadzone)){
            onTime = cycleTime;
            offTime = -1;
        }else if(onPercent >= offPercent){
            onTime = (onPercent / offPercent) * cycleTime;
            offTime = cycleTime;
        }else if(offPercent > onPercent){
            onTime = cycleTime;
            offTime = (offPercent / onPercent) * cycleTime;
        }

        if(onPercent == 0){
            PFMTimer.reset(cycleTime);
            ClockwiseTimer.reset(0);
            CounterClockwiseTimer.reset(0);
        }else if(continuousOutput > 0){
            PFMTimer.reset(onTime + offTime);
            ClockwiseTimer.reset(onTime);
            CounterClockwiseTimer.reset(0);
        }else if(continuousOutput < 0){
            PFMTimer.reset(onTime + offTime);
            ClockwiseTimer.reset(0);
            CounterClockwiseTimer.reset(onTime);
        }
    }

    if(!ClockwiseTimer.isComplete()){
        return Solenoids::CLOCKWISE;
    }else if(!CounterClockwiseTimer.isComplete()){
        return Solenoids::COUNTERCLOCKWISE;
    }else{
        return Solenoids::SOLENOIDS_OFF;
    }
}

CascadedPID::CascadedPID(OutputTransform transform) {
    outputTransform = transform;
}

Solenoids CascadedPID::getStabilization(Data &data) {
    float pidOutput;
    switch(data.target.mode){
        case TargetingMode::ORIENTATION:
            static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
            static PIDMath oVelocityPID = PIDMath(0.006, 0, 0, 7);
            //Normalized error
            error = ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;            
            pidOutput = oVelocityPID.getOutput(
                constrain(orientationPID.getOutput(error), -50, 50)
            );
            break;
        case TargetingMode::VELOCITY:
            static PIDMath velocityPID = PIDMath(0.006, 0, 0, 7);
            pidOutput = velocityPID.getOutput(error);
            break;
        default:
            return Solenoids::SOLENOIDS_OFF;
    }
    return outputTransform.getTransformed(pidOutput);
}

PurePID::PurePID(OutputTransform transform) {
    outputTransform = transform;
}

Solenoids PurePID::getStabilization(Data &data) {
    //TODO
    float pidOutput;
    switch(data.target.mode){
        case TargetingMode::ORIENTATION:
            //Needs to be tuned
            static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
            //Normalized error
            error = ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
            pidOutput = orientationPID.getOutput(error);
            break;
        case TargetingMode::VELOCITY:
            //Needs to be tuned
            static PIDMath velocityPID = PIDMath(0.006, 0, 0, 7);
            pidOutput = velocityPID.getOutput(error);
            break;
        default:
            return Solenoids::SOLENOIDS_OFF;
    }
    return outputTransform.getTransformed(pidOutput);
}

Solenoids BangBang::getStabilization(Data &data) {
    // Default to solenoids off
    Solenoids solenoidState = SOLENOIDS_OFF;
    // Normalized orientation error
    error = ((int)((data.orientation.x - data.target.target) + 540) % 360) - 180;
    // If outside orientation bounds, correct orientation
    if (data.target.mode == ORIENTATION && abs(error) >= 5) {
        if (error < 0 && data.gyro.z < 15) {
           solenoidState = CLOCKWISE;
        }
        else if (error > 0 && data.gyro.z > -15) {
          solenoidState = COUNTERCLOCKWISE;
        }
        else {
            solenoidState = SOLENOIDS_OFF;
        }
        // Set LED based on orientation
        errorLED.setColor({
            (int)(abs(error) * 1.41),       // RED
            0,                              // GREEN
            255 - (int)(abs(error) * 1.41)  // BLUE
        });
    } 
    // If within orientation bounds, control speed to 5 deg/sec
    else if (data.target.mode == VELOCITY || abs(data.gyro.z) >= 5) {
        float velocityError = 0;
        if (data.target.mode == VELOCITY) {
            velocityError = data.gyro.z - data.target.target;
            // Set LED based on velocity
            errorLED.setColor({
                (int)(abs(velocityError) * 5),       // RED
                0,                                   // GREEN
                255 - (int)(abs(velocityError) * 5)  // BLUE
            });
        }
        else {
            errorLED.setColor(colorPresets.green);
        }
        if (velocityError < 0) {
            solenoidState = CLOCKWISE;
        }
        else { 
            solenoidState = COUNTERCLOCKWISE;
        }
    }
    return solenoidState;
}