#include "config.cpp"
#include "data.cpp"
#include "sensors.cpp"
#include "targeter.cpp"
#include "stabilization.cpp"
#include "telemetry.cpp"
#include "time.cpp"

// --- THIS BIT IS TEMPORARY TO MAKE THE RED LINES GO AWAY ---
extern enum IOMode {INPUT, OUTPUT};
extern void pinMode(int pin, IOMode ioMode);
extern enum PinState {LOW, HIGH};
extern void digitalWrite(int pin, PinState pinState);
// --- DELETE IT WHEN WE START USING ARDUINO ---

Config config;
Data data;
Sensors sensors;
TargetPresets targetPresets;
Logger* logger = new OpenLog();
void initPins();
void blinkLEDs();
void updateFlightState();
void stateActions();
void setSolenoids();

void setup() {
    logger->init();
    sensors.imu = new BNO055();
    sensors.gps = new M9N();
    sensors.barometer = new BMP388();
}

void loop() {
    blinkLEDs();
    sensors.imu->collectData(data);
    sensors.gps->collectData(data);
    sensors.barometer->collectData(data);
    updateFlightState();
    stateActions();
    setSolenoids();
    logger->writeTelemetry(data);
}

void initPins() {
    pinMode(config.pins.clockwise, OUTPUT);
    pinMode(config.pins.counterclockwise, OUTPUT);
    pinMode(config.pins.led1, OUTPUT);
    pinMode(config.pins.led2, OUTPUT);
}

void blinkLEDs() {
    static Timer cycleTime = Timer(config.blink.cycleTime);
    static Timer duration = Timer(config.blink.duration);

    if (cycleTime.isComplete()) {
        digitalWrite(config.pins.led1, HIGH);
        digitalWrite(config.pins.led2, HIGH);
        duration.reset();
        cycleTime.reset();
    }
    if (duration.isComplete()) {
        digitalWrite(config.pins.led1, LOW);
        digitalWrite(config.pins.led2, LOW);
    }
}

void updateFlightState() {
    switch (data.state) {
        case STANDBY:
            if (data.gps.pos.alt >= config.targetAltitude) {
                // TODO: Make this resistant to one-time altitude errors
                data.state = FlightState::STABILIZATION;
            }
            break;
        case STABILIZATION:
            if (data.gps.pos.alt <= config.deactivateAltitude) {
                // TODO: Make this resistant to one-time altitude errors
                data.state = FlightState::STABILIZATION;
            }
            break;
        case LANDED:
            break;
    }
}

void stateActions() {
    switch (data.state) {
        case STANDBY:
            data.solenoids = Solenoids::OFF;
            break;
        case STABILIZATION:
            data.target = targetPresets.sun->getTarget(data);
            StabilizationAlgorithm* algorithm = new PID();
            data.solenoids = algorithm->getStabilization(data);
            break;
        case LANDED:
            data.solenoids = Solenoids::OFF;
            break;
    }
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
    }
}