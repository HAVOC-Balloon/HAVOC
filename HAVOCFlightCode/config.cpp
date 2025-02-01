struct Pins {
    const int clockwise = 15;
    const int counterclockwise = 14;
    const int led1 = 29;
    const int led2 = 33;
}

struct Config {
    const Pins pins;
    const int sampleRate = 50; //  ms. This isn't a rate. This is a time. Rate is 20Hz.
    const float seaLevel = 1013.25; // Pa
    const double targetAltitude = 20000; // m
    const double deactivateAltitude = 1000; // m
    const double GPSTick = 1000; // ms?
}