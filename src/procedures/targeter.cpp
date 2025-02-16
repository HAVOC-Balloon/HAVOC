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
    return target;
}