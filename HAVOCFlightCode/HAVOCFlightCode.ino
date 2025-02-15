#include "config.cpp"
#include "data.cpp"
#include "sensors.cpp"
#include "targeter.cpp"
#include "stabilization.cpp"
#include "telemetry.cpp"

Config config;
Data data;
Sensors sensors;
TargetPresets TargetPresets;
Logger logger = OpenLog();
void initPins();
void setSolenoids();

void setup() {
    logger.init();
    sensors.imu = BNO055();
    sensors.gps = M9N();
    sensors.barometer = BMP388();
}

void loop() {
    sensors.imu.collectData(data);
    sensors.gps.collectData(data);
    sensors.barometer.collectData(data);
    data.target = targetPresets.sun.getTarget(data);
    StabilizationAlgorithm algorithm = PID();
    data.solenoids = algorithm.getStabilization(data);
    setSolenoids();
    logger.writeTelemetry(data);
}

void initPins() {
    pinMode(config.pins.clockwise, OUTPUT);
    pinMode(config.pins.counterclockwise, OUTPUT);
    pinMode(config.pins.led1, OUTPUT);
    pinMode(config.pins.led2, OUTPUT);
}

void setSolenoids() {
    switch (data.solenoids) {
        case OFF:
            digitalWrite(config.pins.clockwise, LOW);
            digitalWrite(config.pins.counterclockwise, LOW);
            break;
        case CLOCKWISE:
            digitalWrite(config.pins.clockwise, HIGH);
            digitalWrite(config.pins.counterclockwise, LOW);
            break;
        case COUNTERCLOCKWISE:
            digitalWrite(config.pins.clockwise, LOW);
            digitalWrite(config.pins.counterclockwise, HIGH);
            break;
        default:
            digitalWrite(config.pins.clockwise, LOW);
            digitalWrite(config.pins.counterclockwise, LOW);
    }
}