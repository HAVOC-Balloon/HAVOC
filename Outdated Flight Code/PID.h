#include "Arduino.h"

class Timer{
  private:
    long startTime;
  public:
    Timer();
    void reset();
    long getTime();
};

class CountdownTimer{
  private:
    long length;
    Timer internal;
  public:
    CountdownTimer();
    CountdownTimer(long time);
    void reset(long newTime);
    void reset();
    bool isDone();
    long getTimeLeft();
};