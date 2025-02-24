#pragma once
#include <data.h>

class Logger {
public:
    virtual void init() = 0;
    virtual void writeTelemetry(Data &data) = 0;
};

class OpenLog: public Logger {
public:
    void init();
    void writeTelemetry(Data &data);
};

class SPILogger: public Logger {
    void init();
    void writeTelemetry(Data &data);
};