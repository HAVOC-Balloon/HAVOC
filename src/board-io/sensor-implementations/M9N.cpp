#include <data.h>
#include <board-io/sensors.h>

void M9N::init() {
    // TODO
}

Position M9N::getPosition() {
    // TODO
    // Make sure to convert altitude from mm to m
    return {0, 0, 0};
}

UTCTime M9N::getUTCTime() {
    // TODO
    return {0, 0, 0, 0, 0, 0};
}

int M9N::getSIV() {
    // TODO
    return 0;
}