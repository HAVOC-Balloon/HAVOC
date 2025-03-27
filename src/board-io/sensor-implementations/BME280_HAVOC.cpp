#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>
#include <BME280.h>  

BME280_Class bme; 

void BME280::init() {
    while (!bme.begin(I2C_FAST_MODE_PLUS_MODE)) { 
        // TODO: Handle error here once we get error lights going
        errorLED.setColor(colorPresets.magenta);
        delay(250);
        errorLED.setColor(colorPresets.green);
        delay(250);
    }
    errorLED.setColor(colorPresets.off);

    if(bme.mode()==0) 
    {
        bme.mode(NormalMode); //set to normal use case mode
    }

    bme.setOversampling(TemperatureSensor,Oversample4);
    bme.setOversampling(HumiditySensor,   Oversample4);
    bme.setOversampling(PressureSensor,   Oversample4);

    bme.iirFilter(IIR4);
    bme.inactiveTime(inactive1000ms); //Setting time between measurements to 1 second

    dataReady.setDuration(1000);
}

void BME280::prefetchData() {
    long fetchedTemperature, fetchedHumidity, fetchedPressure;
    bme.getSensorData(fetchedTemperature, fetchedHumidity, fetchedPressure);
    pressure = (float)fetchedPressure / 100.0;
    temperature = (float)fetchedTemperature / 100.0;
    humidity = (float) fetchedHumidity / 100.0;
    altitude = 44330.0*(1.0 - pow(((float) fetchedPressure / 100.0) / 1013.25, 0.1903));
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