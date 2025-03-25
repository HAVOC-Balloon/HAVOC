#include <utilities/pid.h>
#include <math.h>

PIDMath::PIDMath(float kp, float ki, float kd, float deadbandError){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->deadbandError = deadbandError;
    dt = Stopwatch();
}

float PIDMath::getOutput(float error){
    if(abs(error) < deadbandError){
        return 0;
    }
    float time = (float)dt.time();

    d = (error - p) / time;
    i = i + (error * time);
    p = error;
    dt.reset();

    return (p * kp) + (i * ki) + (d * kd);
}