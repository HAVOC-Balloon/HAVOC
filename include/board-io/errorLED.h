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
public:
    ErrorLED(int redPin, int bluePin, int greenPin);
    void initPins();
    void setColor(Color color);
    void colorSweep(Color start, Color end, unsigned long duration);
};