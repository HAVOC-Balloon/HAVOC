#include "data.h"
#include "procedures/stabilization.h"
Solenoids OutputTransform::getTransformed(float continuousOutput) {
    return Solenoids::SOLENOIDS_OFF;
}

Solenoids PFM::getTransformed(float continuousOutput){
    static Timer PWMTimer(0);
    static Timer ClockwiseTimer(0);
    static Timer CounterClockwiseTimer(0);

    if(PWMTimer.isComplete()){
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
            PWMTimer.reset(cycleTime);
            ClockwiseTimer.reset(0);
            CounterClockwiseTimer.reset(0);
        }else if(continuousOutput > 0){
            PWMTimer.reset(onTime + offTime);
            ClockwiseTimer.reset(onTime);
            CounterClockwiseTimer.reset(0);
        }else if(continuousOutput < 0){
            PWMTimer.reset(onTime + offTime);
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

/*
Solenoids BangBangTransform::getTransformed(float continuousOutput){

}
*/

CascadedPID::CascadedPID(OutputTransform transform) {
    outputTransform = transform;
}

Solenoids CascadedPID::getStabilization(Data data) {
    float pidOutput;
    switch(data.target.mode){
        case TargetingMode::ORIENTATION:
            static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
            static PIDMath oVelocityPID = PIDMath(0.006, 0, 0, 7);
            //Normalized error
            error = int(data.target.target - (360 - data.orientation.x)) % 360 - 180;            
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

Solenoids PurePID::getStabilization(Data data) {
    //TODO
    float pidOutput;
    switch(data.target.mode){
        case TargetingMode::ORIENTATION:
            //Needs to be tuned
            static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
            //Normalized error
            error = int(data.target.target - (360 - data.orientation.x)) % 360 - 180;
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

/*
This should just be an output transform
Solenoids PiddedBangBang::getStabilization(Data data){
    float pidOutput;
    switch(data.target.mode){
        case TargetingMode::ORIENTATION:
            //Needs to be tuned
            static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
            error = int(data.target.target - (360 - data.orientation.x)) % 360 - 180;
            pidOutput = orientationPID.getOutput(error);

            break;
    }
}
*/

Solenoids BangBang::getStabilization(Data data) {
    switch(data.target.mode){
        case TargetingMode::ORIENTATION:
            //Normalized error
            error = int(data.target.target - (360 - data.orientation.x)) % 360 - 180;
            if(error > 10){
                return Solenoids::CLOCKWISE;
            }else if(error < -10){
                return Solenoids::COUNTERCLOCKWISE;
            }else{
                return Solenoids::SOLENOIDS_OFF;
            }
            break;
        case TargetingMode::VELOCITY:
            error = data.target.target - data.gyro.z;
            if(error > 7){
                return Solenoids::CLOCKWISE;
            }else if(error < -7){
                return Solenoids::COUNTERCLOCKWISE;
            }else{
                return Solenoids::SOLENOIDS_OFF;
            }
            break;
        default:
            return Solenoids::SOLENOIDS_OFF;
    }
    
    //return Solenoids::SOLENOIDS_OFF;
}