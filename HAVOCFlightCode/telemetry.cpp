class Logger {
public:
    virtual void init() = 0;
    virtual void writeTelemetry() = 0;
};

class OpenLog: Logger {

};

class SPI: Logger {

};