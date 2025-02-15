extern unsigned long millis();

class Stopwatch {
protected:
    unsigned long startTime; // ms
public:
    Stopwatch() {
        startTime = millis();
    }
    unsigned long getStartTime() {
        return startTime;
    }
    unsigned long time() {
        return millis() - startTime;
    }
    void reset() {
        startTime = millis();
    }
};

class Timer: Stopwatch {
protected:
    unsigned long duration; // ms
public:
    Timer(unsigned long duration) : Stopwatch() {
        this->duration = duration;
    }
    bool isComplete() {
        return millis() >= startTime + duration;
    }
    unsigned long timeRemaining() {
        return startTime + duration - millis();
    }
    void setDuration(unsigned long newDuration) {
        duration = newDuration;
    }
};

class Alarm {
protected:
    unsigned long endTime; // ms
public:
    Alarm(unsigned long endTime) {
        this->endTime = endTime;
    }
    unsigned long getEndTime() {
        return endTime;
    }
    bool isComplete() {
        return millis() >= endTime;
    }
    unsigned long timeRemaining() {
        return endTime - millis();
    }
    void setEndTime(unsigned long newEndTime) {
        endTime = newEndTime;
    }
};