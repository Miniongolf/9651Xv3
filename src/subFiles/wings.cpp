#include "subHeads/wings.hpp"

Wings::Wings(char leftPort, char rightPort) {
    leftPiston = new pros::ADIDigitalOut(leftPort);
    rightPiston = new pros::ADIDigitalOut(rightPort);
}

void Wings::extend(WingSide side) {
    switch (side) {
        case L:
            leftPiston->set_value(true);
            break;
        case R:
            rightPiston->set_value(true);
            break;
        case B:
            leftPiston->set_value(true);
            rightPiston->set_value(true);
            break;
        default: // Shouldn't reach default
            break;
    }
}

void Wings::retract(WingSide side) {
    switch (side) {
        case L:
            leftPiston->set_value(false);
            break;
        case R:
            rightPiston->set_value(false);
            break;
        case B:
            leftPiston->set_value(false);
            rightPiston->set_value(false);
            break;
        default: // Shouldn't reach default
            break;
    }
}

        