#include "PID.h"


Timer::Timer()
{
  startTime = (long)millis();
}

void Timer::reset()
{
  startTime = (long)millis();
}

long Timer::getTime()
{
  return (long)millis() - startTime;
}

// -------------------------------------------------------------------------------------------------------------- *

CountdownTimer::CountdownTimer()
{
  internal = Timer();
  length = 0;
}

CountdownTimer::CountdownTimer(long time)
{
  internal = Timer();
  length = time;
}

void CountdownTimer::reset(long newTime)
{
  length = newTime;
  internal.reset();
}

void CountdownTimer::reset()
{
  //length does not change
  internal.reset();
}

bool CountdownTimer::isDone()
{
  return internal.getTime() > length;
}

long CountdownTimer::getTimeLeft()
{
  long timeLeft = length - internal.getTime();
  if(timeLeft < 0)
  {
    timeLeft = -1;
  }
  return timeLeft;
}

