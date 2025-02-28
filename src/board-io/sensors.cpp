#include "board-io/sensors.h"

#include "data.h"

Sensor::Sensor() { init(); }

void IMU::collectData(Data &data) {
  data.acceleration = getAcceleration();
  data.gyro = getGyro();
  data.orientation = getOrientation();
}

void GPSReceiver::collectData(Data &data) {
  auto pos = getPosition();
  auto siv = getSIV();
  auto time = getUTCTime();

  // Only updating if the tick time has passed, thus each has a value
  if (pos.has_value() && time.has_value() && siv.has_value()) {
    data.gps.pos = pos.value();
    data.gps.time = time.value();
    data.gps.SIV = siv.value();

    lastTick = millis();
  }
}

void Barometer::collectData(Data &data) {
  data.atmo.pressure = getPressure();
  data.atmo.temperature = getTemperature();
  data.atmo.alt = getAltitude();
}