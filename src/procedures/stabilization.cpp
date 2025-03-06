#include "data.h"
#include "procedures/stabilization.h"

Solenoids CascadedPID::getStabilization(Data data) {
    // TODO
    static PIDMath velocityPID = PIDMath(0.006, 0, 0, 7);
    static PIDMath orientationPID = PIDMath(1.0, 0, 0, 10);
    return Solenoids::SOLENOIDS_OFF;
}

Solenoids BangBang::getStabilization(Data data) {
    // TODO
    //Create a static timer to use to make sure the solenoids don't change state too often
    //Since its static it will only set once and then gets reset on solenoidTimer.reset() call
    static Timer solenoidTimer(50);

    //If the timer has run out, reset it to the initial time
    //Otherwise, return current solenoid state
    if(solenoidTimer.isComplete()){
        solenoidTimer.reset();
    }else{
        return data.solenoids;
    }
    
    if(data.target.mode == TargetingMode::ORIENTATION){
        //Normalized error
        double error = int(data.target.target - (360 - data.orientation.x)) % 360 - 180;

        if(error > 10){
            return Solenoids::CLOCKWISE;
        }else if(error < -10){
            return Solenoids::COUNTERCLOCKWISE;
        }else{
            return Solenoids::SOLENOIDS_OFF;
        }
    }

    if(data.target.mode == TargetingMode::VELOCITY){
        double error = data.target.target - data.gyro.z;

        if(error > 7){
            return Solenoids::CLOCKWISE;
        }else if(error < -7){
            return Solenoids::COUNTERCLOCKWISE;
        }else{
            return Solenoids::SOLENOIDS_OFF;
        }
    }
    
    return Solenoids::SOLENOIDS_OFF;
}