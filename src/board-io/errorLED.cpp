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

void ErrorLED::setPins(Color color) {
    analogWrite(red, color.red);
    analogWrite(green, color.green);
    analogWrite(blue, color.blue);
}

void ErrorLED::setColor(Color color) {
    currentColor = color;
    setPins(currentColor);
}

void ErrorLED::temporaryColor(Color color) {
    setPins(color);
}

void ErrorLED::clearTemporaryColor() {
    setPins(currentColor);
}

// THIS IS A BLOCKING FUNCTION.
void ErrorLED::timedColor(Color color, unsigned long duration) {
    temporaryColor(color);
    delay(duration);
    clearTemporaryColor();
}

// THIS IS A BLOCKING FUNCTION.
void ErrorLED::colorSweep(Color nextColor, unsigned long duration) {
    Timer sweep = Timer(duration);
    while (!sweep.isComplete()) {
        float progress = sweep.progress();
        // Gain function taken from 
        float a = 0.5*pow(2.0*((progress<0.5)?progress:1.0-progress),2);
        float t = (progress<0.5)?a:1.0-a;
        // End gain function. t is the progress from 0 to 1 of the sweep.
        setPins({
            currentColor.red + (int) ((nextColor.red - currentColor.red) * t),
            currentColor.green + (int) ((nextColor.green - currentColor.green) * t),
            currentColor.blue + (int) ((nextColor.blue - currentColor.blue) * t)
        });
        delay(1);
    }
    setColor(nextColor);
}