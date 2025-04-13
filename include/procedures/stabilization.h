#pragma once
#include <utilities/pid.h>

#include "data.h"
#include "havoc.h"

class OutputTransform {
 public:
  virtual Solenoids getTransformed(float continuousOutput);
};

class PFM : public OutputTransform {
 private:
  const unsigned long cycleTime = config.solenoidCycleTime;
  const float deadzone = 1.5 / (1000.0 / cycleTime);

  float onPercent;
  float offPercent;
  unsigned long onTime;
  unsigned long offTime;

 public:
  virtual Solenoids getTransformed(float continuousOutput);
};

class StabilizationAlgorithm {
 protected:
  double error;  // Either deg or deg/s depending on targeting mode
 public:
  virtual Solenoids getStabilization(Data &data) = 0;
};

class CascadedPID : public StabilizationAlgorithm {
 private:
  OutputTransform outputTransform;

 public:
  CascadedPID(OutputTransform *transform);
  Solenoids getStabilization(Data &data);
};

class PurePID : public StabilizationAlgorithm {
 private:
  OutputTransform *outputTransform;

 public:
  PurePID(OutputTransform *transform);
  ~PurePID();
  Solenoids getStabilization(Data &data);
};

class BangBang : public StabilizationAlgorithm {
 private:
  enum { STABLE, UNSTABLE } status = STABLE;

 public:
  Solenoids getStabilization(Data &data);
};
