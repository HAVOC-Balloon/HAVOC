#include "data.cpp"

class StabilizationAlgorithm {
public:
    virtual Solenoids getStabilization(Data data) = 0;
};

class PID: public StabilizationAlgorithm {
public:
    Solenoids getStabilization(Data data) {
        // TODO
    }
};

class BangBang: public StabilizationAlgorithm {
public:
    Solenoids getStabilization(Data data) {
        // TODO
    }
};