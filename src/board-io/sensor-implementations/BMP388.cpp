#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>

void BMP388::init() {
    bmp.begin_I2C();
    if (!bmp.begin_I2C()) {
        // TODO: Handle error here once we get error lights going
    }
    
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
}

float BMP388::getPressure() {
    return bmp.readPressure();
}

float BMP388::getTemperature() {
    return bmp.readTemperature();
}

float BMP388::getAltitude() {
    return bmp.readAltitude(config.seaLevelPressure);
}