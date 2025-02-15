#include "data.cpp"

class Targeter {
public:
    virtual Target getTarget(Data data) = 0;
};

class ConstantTargeter: Targeter {
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

class SolarTargeter: Targeter {
public:
    Target getTarget(Data data) {
        Target target;
        target.mode = ORIENTATION;        
        target.target = 0; // TODO: SOLAR POSITIONING CODE HERE
        return target;
    }
};

class GPSTargeter: Targeter {
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
        return target
    }
};

struct TargetPresets {
    ConstantTargeter north = ConstantTargeter({ORIENTATION, 0}),
    ConstantTargeter east = ConstantTargeter({ORIENTATION, 90}),
    ConstantTargeter south = ConstantTargeter({ORIENTATION, 180}),
    ConstantTargeter west = ConstantTargeter({ORIENTATION, 270}),
    SolarTargeter sun = SolarTargeter(),
    GPSTargeter swirll = GPSTargeter({ORIENTATION, {
        34.725032, -86.646366, 200
    }})
    GPSTargeter uah = GPSTargeter({ORIENTATION, {
        34.7246131, -86.6397583, 190
    }})
};