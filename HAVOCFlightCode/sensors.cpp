#include "data.cpp"

struct Sensors {
    IMU imu;
    GPS gps;
    Barometer barometer;
}

class Sensor {
public:
    Sensor() {
        init();
    }
    virtual void init() = 0;
    virtual void collectData(Data data) = 0;
}

class IMU: Sensor {

}

class GPS: Sensor {

}

class Barometer: Sensor {

}

class BNO055: IMU {

}

class M9N: GPS {

}

class BMP388: Barometer {

}