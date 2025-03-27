#include <Arduino.h>
#include "utilities/time.h"
#include "board-io/errorLED.h"

ColorPresets colorPresets;

ErrorLED::ErrorLED(int redPin, int greenPin, int bluePin) {
    red = redPin;
    blue = bluePin;
    green = greenPin;
}

void ErrorLED::initPins() {
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    setColor(colorPresets.off);
}

void ErrorLED::setColor(Color color) {
    analogWrite(red, color.red);
    analogWrite(green, color.green);
    analogWrite(blue, color.blue);
}

// THIS IS A BLOCKING FUNCTION.
void ErrorLED::colorSweep(Color start, Color end, unsigned long duration) {
    setColor(start);
    Timer sweep = Timer(duration);
    while (!sweep.isComplete()) {
        float progress = sweep.progress();
        // Gain function taken from 
        float a = 0.5*pow(2.0*((progress<0.5)?progress:1.0-progress),2);
        float t = (progress<0.5)?a:1.0-a;
        // End gain function. t is the progress from 0 to 1 of the sweep.
        setColor({
            start.red + (int) ((end.red - start.red) * t),
            start.green + (int) ((end.green - start.green) * t),
            start.blue + (int) ((end.blue - start.blue) * t)
        });
        delay(1);
    }
    setColor(end);
}