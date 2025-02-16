#pragma once
#include "data.h"

class StabilizationAlgorithm {
public:
    virtual Solenoids getStabilization(Data data) = 0;
};

class PID: public StabilizationAlgorithm {
public:
    Solenoids getStabilization(Data data);
};

class BangBang: public StabilizationAlgorithm {
public:
    Solenoids getStabilization(Data data);
};