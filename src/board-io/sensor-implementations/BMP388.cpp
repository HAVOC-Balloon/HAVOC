#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>

void BMP388::init() {
    while (!bme.begin()) {
        // TODO: Handle error here once we get error lights going
        Serial.println("BME not connected");
        delay(200);
    }
    
    //bme.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    //bme.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    //bme.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
}

float BMP388::getPressure() {
    return bme.readPressure();
}

float BMP388::getTemperature() {
    return bme.readTemperature();
}

float BMP388::getAltitude() {
    return bme.readAltitude(config.seaLevelPressure);
}