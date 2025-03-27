#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>
#include <BME280.h>  

BME280_Class bme; 

void BME280::init() {
    while (!bme.begin(I2C_FAST_MODE_PLUS_MODE)) { 
        // TODO: Handle error here once we get error lights going
        errorLED.timedColor(colorPresets.magenta, 250);
        errorLED.timedColor(colorPresets.green, 250);
    }

    bme.mode(ForcedMode); // Force one measurement to begin

    bme.setOversampling(TemperatureSensor,Oversample16);
    bme.setOversampling(HumiditySensor,   Oversample16);
    bme.setOversampling(PressureSensor,   Oversample16);

    bme.iirFilter(IIR16);
    //bme.inactiveTime(inactive1000ms); //Setting time between measurements to 1 second

    dataReady.setDuration(1000);
}

bool BME280::prefetchData() {
    // If the BME is in sleep mode it means a measurement has been completed
    if (bme.mode() == SleepMode) {
        // Therefore we collect the data here...
        long fetchedTemperature, fetchedHumidity, fetchedPressure;
        bme.getSensorData(fetchedTemperature, fetchedHumidity, fetchedPressure);
        pressure = (float)fetchedPressure / 100.0;
        temperature = (float)fetchedTemperature / 100.0;
        humidity = (float) fetchedHumidity / 100.0;
        altitude = 44330.0*(1.0 - pow(((float) fetchedPressure / 100.0) / 1013.25, 0.1903));
        // ...then the BME to forced mode to take another measurement
        bme.mode(ForcedMode);
        return true;
    }
    return false;
}

std::optional<float> BME280::getPressure() {
    return pressure;
}

std::optional<float> BME280::getTemperature() {
    return temperature;
}

std::optional<float> BME280::getAltitude() { 
    return altitude;
}

std::optional<float> BME280::getHumidity(){
    return humidity;
}