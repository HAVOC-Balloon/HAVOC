#include <havoc.h>
#include <procedures/stabilization.h>
#include <utilities/time.h>

Config config;
Data data;
Sensors sensors;
TargetPresets targetPresets;
SPILogger logger = SPILogger();
ErrorLED errorLED =
    ErrorLED(config.pins.redLed, config.pins.greenLed, config.pins.blueLed);

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
  static Timer stateTimer(0);
  switch (data.state) {
    case STANDBY:
      if (data.gps.pos.alt >= config.targetAltitude && data.gps.SIV >= 3) {
        data.state = PRESTABILIZATION;
        stateTimer.reset(config.waitTimes.stabilization);
      }
      data.state = STABILIZATION; 
      break;
    case PRESTABILIZATION:
      if (data.gps.pos.alt >= config.targetAltitude && data.gps.SIV >= 3) {
        if (stateTimer.isComplete()) {
          data.state = STABILIZATION;
        }
      } else {
        data.state = STANDBY;
      }
      break;
    case STABILIZATION:
      if (data.gps.pos.alt <= config.deactivateAltitude && data.gps.SIV >= 3) {
        data.state = PRELANDED;
        stateTimer.reset(config.waitTimes.landed);
      }
      break;
    case PRELANDED:
      if (data.gps.pos.alt <= config.deactivateAltitude && data.gps.SIV >= 3) {
        if (stateTimer.isComplete()) {
          data.state = LANDED;
        }
      } else {
        data.state = STABILIZATION;
      }
      break;
    case LANDED:
      break;
  }
}

void stateActions() {
  Solenoids requestedSolenoidState;
  //PFM *transform = new PFM(); 
  switch (data.state) {
    case FlightState::STANDBY:
      break;
    case FlightState::PRESTABILIZATION:
      digitalWrite(0, HIGH);
      digitalWrite(1, HIGH);
      break;
    case FlightState::STABILIZATION:
      data.target = targetPresets.north->getTarget(data); 
      //requestedSolenoidState = CascadedPID(new PFM()).getStabilization(data);
      requestedSolenoidState = PurePID(new PFM()).getStabilization(data); 
      //data.target = targetPresets.north->getTarget(data);
      //requestedSolenoidState = BangBang().getStabilization(data);
      setSolenoids(requestedSolenoidState);
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
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