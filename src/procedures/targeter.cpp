#include "data.h"
#include "procedures/targeter.h"


ConstantTargeter::ConstantTargeter(Target target) {
    this->target = target;
}

Target ConstantTargeter::getTarget(Data data) {
    return target;
}

Target SolarTargeter::getTarget(Data data) {
    Target target;
    target.mode = TargetingMode::ORIENTATION;        
    target.target = 0; // TODO: SOLAR POSITIONING CODE HERE
    return target;
}

GPSTargeter::GPSTargeter(Position position) {
    this->position = position;
}

Target GPSTargeter::getTarget(Data data) {
    Target target;
    target.mode = TargetingMode::ORIENTATION;
    target.target = 0; // TODO: GPS CODE HERE
    /*
    setTime(Hour, Minute, Second, Day, Month, Year); 
    t_object = now();  

    SolarPosition location(latitude, longitude);
    int solar_Azimuth = ((int)location.getSolarAzimuth(t));
    */
   // P.S. This is the GPS targeter class; it should be in SolarTargeter!


    return target;
}