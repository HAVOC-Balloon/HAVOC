#include <utilities/time.h>
#include <procedures/stabilization.h>
#include <havoc.h>

Config config;
Data data;
Sensors sensors;
TargetPresets targetPresets;
SPILogger logger = SPILogger();
ErrorLED errorLED = ErrorLED(config.pins.redLed, config.pins.greenLed, config.pins.blueLed);

void initPins() {
    pinMode(config.pins.clockwise, OUTPUT);
    pinMode(config.pins.counterclockwise, OUTPUT);
    setSolenoids(SOLENOIDS_OFF);
    pinMode(config.pins.sideLed, OUTPUT);
    digitalWrite(config.pins.sideLed, HIGH);
    errorLED.initPins();
}

void happyHavocLightDance() {
    errorLED.colorSweep(colorPresets.yellow, 500);
    errorLED.colorSweep(colorPresets.red, 500);
    errorLED.colorSweep(colorPresets.magenta, 500);
    errorLED.colorSweep(colorPresets.blue, 500);
    errorLED.colorSweep(colorPresets.cyan, 500);
    errorLED.colorSweep(colorPresets.green, 500);
}

void blinkLEDs() {
    static Timer cycleTime = Timer(config.blink.cycleTime);
    static Timer duration = Timer(config.blink.duration);

    if (cycleTime.complete()) {
        digitalWrite(config.pins.sideLed, HIGH);
        duration.reset();
        cycleTime.reset();
    }
    if (duration.complete()) {
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
            data.target = targetPresets.north->getTarget(data);
            requestedSolenoidState = BangBang().getStabilization(data);
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

    if (solenoidState == data.solenoids || canToggleSolenoids.incomplete()) {
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