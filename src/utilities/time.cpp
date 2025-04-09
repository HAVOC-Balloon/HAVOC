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

bool Timer::complete() {
    return millis() >= startTime + duration;
}

bool Timer::incomplete() {
    return !complete();
}

unsigned long Timer::timeRemaining() {
    return startTime + duration - millis();
}

float Timer::progress() {
    return (float) time() / duration;
}

void Timer::setDuration(unsigned long newDuration) {
    duration = newDuration;
}

void Timer::reset(){
    startTime = millis();
}

void Timer::reset(unsigned long newDuration){
    setDuration(newDuration);
    reset();
}

Alarm::Alarm(unsigned long endTime) {
    this->endTime = endTime;
}

unsigned long Alarm::getEndTime() {
    return endTime;
}

bool Alarm::complete() {
    return millis() >= endTime;
}

bool Alarm::incomplete() {
    return !complete();
}

unsigned long Alarm::timeRemaining() {
    return endTime - millis();
}

void Alarm::setEndTime(unsigned long newEndTime) {
    endTime = newEndTime;
}
