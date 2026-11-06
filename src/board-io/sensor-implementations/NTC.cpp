#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>

// Thermistor / circuit parameters
const float R_FIXED  = 10000.0f;   // fixed divider resistor (ohms) - use measured value for best accuracy
const float R_NOMINAL = 10000.0f;  // NTC resistance at 25 °C
const float T_NOMINAL = 25.0f;     // °C
const float BETA      = 3977.0f;   // B25/85 for NTCLE100E3103xB0 (check your exact part's datasheet)

// ADC settings
const int ADC_BITS = 12;
const int ADC_MAX  = (1 << ADC_BITS) - 1;
const int NUM_SAMPLES = 16;

void NTC::init(){
  analogReadResolution(12);
  analogReadAveraging(4);
}

float NTC::readCelsius(int pin) {
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
}

bool NTC::prefetchData(){
  return true; //Since its just a read you don't need to prefetch data, but it yelled at me to
}

void NTC::collectData(Data &data){
  data.temp1 = readCelsius(config.pins.thermistor1);
  data.temp2 = readCelsius(config.pins.thermistor2);

}