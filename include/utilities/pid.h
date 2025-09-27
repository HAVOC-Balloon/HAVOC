#include <utilities/time.h>

class PIDMath {
private:
    float p = 0.0;
    float i = 0.0;
    float d = 0.0;
    float kp;
    float ki;
    float kd;
    float deadbandError;
    Stopwatch dt;
public:
    PIDMath(float kp, float ki, float kd, float deadbandError);
    float getOutput(float error);
};