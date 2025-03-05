#include <Arduino.h>
#include <utilities/time.h>

Stopwatch::Stopwatch() {
    startTime = millis();
}

unsigned long Stopwatch::getStartTime() {
    return startTime;
}

unsigned long Stopwatch::time() {
    return millis() - startTime;
}

void Stopwatch::reset() {
    startTime = millis();
}

Timer::Timer(unsigned long duration) : Stopwatch() {
    this->duration = duration;
}

bool Timer::isComplete() {
    return millis() >= startTime + duration;
}

unsigned long Timer::timeRemaining() {
    return startTime + duration - millis();
}

void Timer::setDuration(unsigned long newDuration) {
    duration = newDuration;
}

Alarm::Alarm(unsigned long endTime) {
    this->endTime = endTime;
}

unsigned long Alarm::getEndTime() {
    return endTime;
}

bool Alarm::isComplete() {
    return millis() >= endTime;
}

unsigned long Alarm::timeRemaining() {
    return endTime - millis();
}

void Alarm::setEndTime(unsigned long newEndTime) {
    endTime = newEndTime;
}
