#pragma once
#include "data.h"
#include <utilities/pid.h>
#include "havoc.h"

class OutputTransform {
public:
    Solenoids getTransformed(float continuousOutput);
};

class PFM: public OutputTransform {
private:
    const unsigned long cycleTime = config.solenoidCycleTime;
    const float deadzone = 1.5 / (1000.0 / cycleTime);
    
    float onPercent;
    float offPercent;
    unsigned long onTime;
    unsigned long offTime;
public:
    Solenoids getTransformed(float continuousOutput);
};

class BangBangTransform: public OutputTransform{
public:
    Solenoids getTransformed(float continuousOutput);
};

class StabilizationAlgorithm {
protected:
    double error;
public:
    // TODO: Change to pass by reference after flight
    virtual Solenoids getStabilization(Data data) = 0;
};


class CascadedPID: public StabilizationAlgorithm {
private:
    OutputTransform outputTransform;
public:
    CascadedPID(OutputTransform transform);
    Solenoids getStabilization(Data data);
};

class PurePID: public StabilizationAlgorithm {
private:
    OutputTransform outputTransform;
public:
    PurePID(OutputTransform transform);
    Solenoids getStabilization(Data data);
};

class PiddedBangBang: public StabilizationAlgorithm{
public:
    Solenoids getStabilization(Data data);
};

class BangBang: public StabilizationAlgorithm {
public:
    Solenoids getStabilization(Data data);
};

