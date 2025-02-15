#include "data.cpp"

class Targeter {
public:
    virtual Target getTarget(Data data) = 0;
};

class ConstantTargeter: public Targeter {
private:
    Target target;
public:
    ConstantTargeter(Target target) {
        this->target = target;
    }
    Target getTarget(Data data) {
        return target;
    }
};

class SolarTargeter: public Targeter {
public:
    Target getTarget(Data data) {
        Target target;
        target.mode = ORIENTATION;        
        target.target = 0; // TODO: SOLAR POSITIONING CODE HERE
        return target;
    }
};

class GPSTargeter: public Targeter {
private:
    Position position;
public:
    GPSTargeter(Position position) {
        this->position = position;
    }
    Target getTarget(Data data) {
        Target target;
        target.mode = ORIENTATION;
        target.target = 0; // TODO: GPS CODE HERE
        return target;
    }
};

struct TargetPresets {
    Targeter* north = new ConstantTargeter({ORIENTATION, 0});
    Targeter* east = new ConstantTargeter({ORIENTATION, 90});
    Targeter* south = new ConstantTargeter({ORIENTATION, 180});
    Targeter* west = new ConstantTargeter({ORIENTATION, 270});
    Targeter* rotationLimiter = new ConstantTargeter({VELOCITY, 0});
    Targeter* slowClockwise = new ConstantTargeter({VELOCITY, -10});
    Targeter* slowCounterclockwise = new ConstantTargeter({VELOCITY, 10});
    Targeter* fastClockwise = new ConstantTargeter({VELOCITY, -40});
    Targeter* fastCounterclockwise = new ConstantTargeter({VELOCITY, 40});
    Targeter* sun = new SolarTargeter();
    Targeter* swirll = new GPSTargeter({34.725032, -86.646366, 200});
    Targeter* uah = new GPSTargeter({34.7246131, -86.6397583, 190});
};