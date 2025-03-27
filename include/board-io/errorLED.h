struct Color {
    int red = 0;
    int green = 0;
    int blue = 0;
};

struct ColorPresets {
    Color off {0, 0, 0};
    Color white {255, 255, 255};
    Color red {255, 0, 0};
    Color green {0, 255, 0};
    Color blue {0, 0, 255};
    Color cyan {0, 255, 255};
    Color magenta {255, 0, 255};
    Color yellow {255, 255, 0};
};

extern ColorPresets colorPresets;

class ErrorLED {
private:
    int red;
    int blue;
    int green;
    Color currentColor;
    void setPins(Color color);
public:
    ErrorLED(int redPin, int bluePin, int greenPin);
    void initPins();
    // Primary function to set the color
    void setColor(Color color);
    // Pair of functions to set a temporary color
    void temporaryColor(Color color);
    void clearTemporaryColor();
    // Blocking functions for specific animations and such
    void timedColor(Color color, unsigned long duration);
    void colorSweep(Color nextColor, unsigned long duration);
};