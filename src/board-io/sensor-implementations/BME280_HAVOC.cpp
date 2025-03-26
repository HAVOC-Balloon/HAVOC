#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>

void BME280::init() {
    while (!BME280.begin(I2C_FAST_MODE_PLUS_MODE)) { 
        // TODO: Handle error here once we get error lights going
        Serial.println("BME not connected");
        delay(200);
    }

    BME280.setOversampling(TemperatureSensor,Oversample16);
    BME280.setOversampling(HumiditySensor,   Oversample16);
    BME280.setOversampling(PressureSensor,   Oversample16);
}

float BME288::getPressure() {
    return 0;
}

float BME288::getTemperature() {
    return 0;
}

float BME288::getAltitude() {
    return 0;
}

float BME280::getHumidity(){

    return 0;
}