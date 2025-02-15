#include <data.cpp>

class Logger {
public:
    virtual void init() = 0;
    virtual void writeTelemetry(Data data) = 0;
};

class OpenLog: public Logger {
public:
    void init() {
        // TODO: INIT CODE FOR OPENLOG
    }
    void writeTelemetry(Data data) {
        // TODO: TELEMETRY CODE FOR OPENLOG
    }
};

class SPI: public Logger {
    void init() {
        // TODO: INIT CODE FOR SPI LOGGER
    }
    void writeTelemetry(Data data) {
        // TODO: TELEMETRY CODE FOR SPI LOGGER
    }
};