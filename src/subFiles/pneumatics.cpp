#include "subHeads/pneumatics.hpp"

// PistonGroup class constructors
PistonGroup::PistonGroup(const std::vector<pros::ADIDigitalOut> pistonsVec)
    : pistonsVec(pistonsVec) {}

PistonGroup::PistonGroup(const std::vector<uint8_t> ports) {
    for (std::uint8_t port : ports) {
        this->pistonsVec.push_back(pros::ADIDigitalOut(port));
    }
}

PistonGroup::PistonGroup(uint8_t port) {
    this->pistonsVec.push_back(pros::ADIDigitalOut(port));
}

// PistonGroup extend
void PistonGroup::extend() {
    for (pros::ADIDigitalOut piston : this->pistonsVec) {
        piston.set_value(true);
    }
    this->isExtended = true;
}

// PistonGroup retract
void PistonGroup::retract() {
    for (pros::ADIDigitalOut piston : this->pistonsVec) {
        piston.set_value(false);
    }
    this->isExtended = false;
}

// PistonGroup toggle
void PistonGroup::toggle() {
    for (pros::ADIDigitalOut piston : this->pistonsVec) {
        piston.set_value(!this->isExtended);
    }
    this->isExtended = !isExtended;
}