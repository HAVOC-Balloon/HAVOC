#include "data.cpp"

class StabilizationAlgorithm {
public:
    virtual Solenoids getStabilization(Data data) = 0;
}

class PID: StabilizationAlgorithm {

}

class BangBang: StabilizationAlgorithm {
    
}