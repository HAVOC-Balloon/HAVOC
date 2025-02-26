#include "board-io/sensors.h"

#include "data.h"

Sensor::Sensor() { init(); }

void IMU::collectData(Data data) {
  data.acceleration = getAcceleration();
  data.gyro = getGyro();
  data.orientation = getOrientation();
}

void GPSReceiver::collectData(Data* data) {
  // Ensuring each has a value
  auto pos = getPosition();
  if (pos.has_value()) data->gps.pos = pos.value();
  auto time = getUTCTime();
  if (time.has_value()) data->gps.time = time.value();
  auto siv = getSIV();
  if (siv.has_value()) data->gps.SIV = siv.value();

  lastTick = millis();
}

void Barometer::collectData(Data data) {
  data.atmo.pressure = getPressure();
  data.atmo.temperature = getTemperature();
  data.atmo.alt = getAltitude();
}