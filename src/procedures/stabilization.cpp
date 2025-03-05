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
    return Solenoids::SOLENOIDS_OFF;
}