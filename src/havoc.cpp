#include <utilities/time.h>
#include <procedures/stabilization.h>
#include <havoc.h>

Config config;
Data data;
Sensors sensors;
TargetPresets targetPresets;
Logger* logger = new SPILogger();

void initPins() {
    pinMode(config.pins.clockwise, OUTPUT);
    pinMode(config.pins.counterclockwise, OUTPUT);
    pinMode(config.pins.sideLed, OUTPUT);
}

void blinkLEDs() {
    static Timer cycleTime = Timer(config.blink.cycleTime);
    static Timer duration = Timer(config.blink.duration);

    if (cycleTime.isComplete()) {
        digitalWrite(config.pins.sideLed, HIGH);
        duration.reset();
        cycleTime.reset();
    }
    if (duration.isComplete()) {
        digitalWrite(config.pins.sideLed, LOW);
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
    Solenoids requestedSolenoidState;
    switch (data.state) {
        case FlightState::STANDBY:
            setSolenoids(SOLENOIDS_OFF);
            break;
        case FlightState::STABILIZATION:
            data.target = targetPresets.sun->getTarget(data);
            // data.solenoids = CascadedPID().getStabilization(data);
            requestedSolenoidState = CascadedPID(PFM()).getStabilization(data);
            setSolenoids(requestedSolenoidState);
            break;
        case FlightState::LANDED:
            setSolenoids(SOLENOIDS_OFF);
            break;
        default:
            setSolenoids(SOLENOIDS_OFF);
    }
}

void setSolenoids(Solenoids solenoidState) {
    static Timer canToggleSolenoids = Timer(config.solenoidCycleTime);

    if (solenoidState == data.solenoids || !canToggleSolenoids.isComplete()) {
        return;
    }

    data.solenoids = solenoidState;
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
    canToggleSolenoids.reset();
}