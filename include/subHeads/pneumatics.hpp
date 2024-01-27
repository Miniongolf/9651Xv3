#pragma once

#include <memory>

#include "pros/adi.hpp"

class PistonGroup {
    public:
        // Piston Group class constructor
        PistonGroup(const std::vector<pros::ADIDigitalOut> pistonsVec);

        // Piston Group class constructor
        PistonGroup(const std::vector<uint8_t> ports);

        void extend();
        void retract();
        void toggle();

        void set_value(bool extended);
        
        bool isExtended = false;

    private:
        std::vector<pros::ADIDigitalOut> pistonsVec;
};