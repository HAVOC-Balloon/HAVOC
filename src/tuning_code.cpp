#include <Arduino.h>

#include "havoc.h"

const int thermistor1Pin = A11;
const int thermistor2Pin = A16;

// Thermistor / circuit parameters
const float R_FIXED  = 10000.0f;   // fixed divider resistor (ohms) - use measured value for best accuracy
const float R_NOMINAL = 10000.0f;  // NTC resistance at 25 °C
const float T_NOMINAL = 25.0f;     // °C
const float BETA      = 3977.0f;   // B25/85 for NTCLE100E3103xB0 (check your exact part's datasheet)

// ADC settings
const int ADC_BITS = 12;
const int ADC_MAX  = (1 << ADC_BITS) - 1;
const int NUM_SAMPLES = 16;

float readCelsius(int pin) {
  // Average several samples to reduce noise
  uint32_t sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
    delayMicroseconds(100);
  }
  float adc = sum / (float)NUM_SAMPLES;

  // Guard against open/shorted sensor
  if (adc < 1.0f || adc > ADC_MAX - 1.0f) return NAN;

  // Divider: 3.3V -- R_FIXED -- node -- NTC -- GND
  float rNtc = R_FIXED * adc / (ADC_MAX - adc);

  // Beta equation
  float tK = 1.0f / (1.0f / (T_NOMINAL + 273.15f) + logf(rNtc / R_NOMINAL) / BETA);
  return tK - 273.15f;
  Serial.println("hi");
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(thermistor1Pin, INPUT);
  pinMode(thermistor2Pin, INPUT);
  analogReadResolution(12);
  analogReadAveraging(4);
  initPins();
  logger.init();
  sensors.barometer.init();
  sensors.imu.init();
  // Disable GPS for tuning
  //sensors.gps.init();
  setSolenoids(SOLENOIDS_OFF);
  delay(500);
  for(int i = 0; i < 4; i++){
    setSolenoids(CLOCKWISE);
    delay(50);
    setSolenoids(SOLENOIDS_OFF);
    delay(200);
    setSolenoids(COUNTERCLOCKWISE);
    delay(50);
    setSolenoids(SOLENOIDS_OFF);
    delay(200);
    Serial.println("yello");
  }
  
  happyHavocLightDance();
}

void loop() {
  static Timer loopTimer(config.loopCycleTime);
  data.packetCount += 1;
  data.missionTime = millis();
  blinkLEDs();
  sensors.imu.collectData(data);
  //sensors.gps.collectData(data);
  sensors.barometer.collectData(data);
  // Set to stabilization for tuning
  data.gps.pos.alt = 24000;
  data.state = FlightState::STABILIZATION;
  stateActions();
  logger.writeTelemetry(data);
  while (!loopTimer.isComplete()) {}
  loopTimer.reset();

  float t1 = readCelsius(thermistor1Pin);
  float t2 = readCelsius(thermistor2Pin);

  Serial.printf("NTC1: %.2f C   NTC2: %.2f C\n", t1, t2);
  delay(500);
}
