#pragma once
#include "data.h"

class Targeter {
public:
    // TODO: Change to pass by reference after flight
    virtual Target getTarget(Data data) = 0;
};

class ConstantTargeter: public Targeter {
private:
    Target target;
public:
    ConstantTargeter(Target target);
    Target getTarget(Data data);
};

class SolarTargeter: public Targeter {
public:
    Target getTarget(Data data);
};

class GPSTargeter: public Targeter {
private:
    Position position;
public:
    GPSTargeter(Position position);
    Target getTarget(Data data);
};

struct TargetPresets {
    Targeter* north = new ConstantTargeter({TargetingMode::ORIENTATION, 0});
    Targeter* east = new ConstantTargeter({TargetingMode::ORIENTATION, 90});
    Targeter* south = new ConstantTargeter({TargetingMode::ORIENTATION, 180});
    Targeter* west = new ConstantTargeter({TargetingMode::ORIENTATION, 270});
    Targeter* rotationLimiter = new ConstantTargeter({TargetingMode::VELOCITY, 0});
    Targeter* slowClockwise = new ConstantTargeter({TargetingMode::VELOCITY, -10});
    Targeter* slowCounterclockwise = new ConstantTargeter({TargetingMode::VELOCITY, 10});
    Targeter* fastClockwise = new ConstantTargeter({TargetingMode::VELOCITY, -40});
    Targeter* fastCounterclockwise = new ConstantTargeter({TargetingMode::VELOCITY, 40});
    Targeter* sun = new SolarTargeter();
    Targeter* swirll = new GPSTargeter({34.725032, -86.646366, 200});
    Targeter* uah = new GPSTargeter({34.7246131, -86.6397583, 190});
};