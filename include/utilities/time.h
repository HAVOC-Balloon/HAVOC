#pragma once

class Stopwatch {
protected:
    unsigned long startTime;
public:
    Stopwatch();
    unsigned long getStartTime();
    unsigned long time();
    void reset();
};

class Timer: public Stopwatch {
protected:
    unsigned long duration; // ms
public:
    Timer(unsigned long duration);
    bool isComplete();
    unsigned long timeRemaining();
    float progress();
    void setDuration(unsigned long newDuration);
    void reset();
    void reset(unsigned long newDuration);
};

class Alarm {
protected:
    unsigned long endTime; // ms
public:
    Alarm(unsigned long endTime);
    unsigned long getEndTime();
    bool isComplete();
    unsigned long timeRemaining();
    void setEndTime(unsigned long newEndTime);
};