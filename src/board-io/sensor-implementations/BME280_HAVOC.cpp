#include <data.h>
#include <board-io/sensors.h>
#include <havoc.h>
#include <BME280.h>  

BME280_Class bme; 

void BME280::init() {
    while (!bme.begin(I2C_FAST_MODE_PLUS_MODE)) { 
        // TODO: Handle error here once we get error lights going
        Serial.println("BME not connected");
        delay(200);
    }

    if(bme.mode()==0) 
    {
        bme.mode(NormalMode); //set to normal use case mode
    }

    bme.setOversampling(TemperatureSensor,Oversample16);
    bme.setOversampling(HumiditySensor,   Oversample16);
    bme.setOversampling(PressureSensor,   Oversample16);

    bme.iirFilter(IIR16);
    bme.inactiveTime(inactive1000ms); //Setting time between measurements to 1 second
}

//NOTE: The way this sensor library works requires the use of the methoed "getSensorData" which returns 
//humidity, pressure, and temperature. We could rewrite the barometer class to better match this syle, or we can
//keep the implementation below that allows control over each variable. Discuss later...

float BME280::getPressure() {
    long temperature, humidity, pressure;      
    bme.getSensorData(temperature,humidity,pressure);
    pressure = pressure/100.0; 
    return pressure;
}

float BME280::getTemperature() {
    long temperature, humidity, pressure;      
    bme.getSensorData(temperature,humidity,pressure);
    temperature = temperature/100.0; 
    return temperature;
}

float BME280::getAltitude() { 
    static float Altitude;
    long temp, hum, press;
    bme.getSensorData(temp, hum, press);
    Altitude = 44330.0*(1.0 - pow(((float)press / 100.0) / 1013.25, 0.1903)); // Convert into altitude in meters
    return(Altitude);
}

float BME280::getHumidity(){
    long temperature, humidity, pressure;      
    bme.getSensorData(temperature,humidity,pressure);
    humidity = humidity/100.0; 
    return humidity;
}