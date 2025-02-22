#include <time/time.h>
#include <procedures/stabilization.h>
#include <havoc.h>

Config config;
Data data;
Sensors sensors;
TargetPresets targetPresets;
Logger* logger = new OpenLog();

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
                data.state = STABILIZATION;
            }
            break;
        case STABILIZATION:
            if (data.gps.pos.alt <= config.deactivateAltitude) {
                // TODO: Make this resistant to one-time altitude errors
                data.state = LANDED;
            }
            break;
        case LANDED:
            break;
    }
}

void stateActions() {
    switch (data.state) {
        case FlightState::STANDBY:
            data.solenoids = SOLENOIDS_OFF;
            break;
        case FlightState::STABILIZATION:
            data.target = targetPresets.sun->getTarget(data);
            data.solenoids = PID().getStabilization(data);
            break;
        case FlightState::LANDED:
            data.solenoids = SOLENOIDS_OFF;
            break;
        default:
            data.solenoids = SOLENOIDS_OFF;
    }
}

void setSolenoids() {
    switch (data.solenoids) {
        case SOLENOIDS_OFF:
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