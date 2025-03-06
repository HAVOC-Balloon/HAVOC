#pragma once
#include "data.h"
#include <utilities/pid.h>

class StabilizationAlgorithm {
public:
    virtual Solenoids getStabilization(Data data) = 0;
};

class CascadedPID: public StabilizationAlgorithm {
private:
    //OutputTransform outputTranform;
public:
    //CascadedPID(OutputTransform transform);
    Solenoids getStabilization(Data data);
};

class BangBang: public StabilizationAlgorithm {
public:
    Solenoids getStabilization(Data data);
};