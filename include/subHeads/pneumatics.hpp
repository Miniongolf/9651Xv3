#pragma once

#include <memory>

#include "pros/adi.hpp"

class PistonGroup {
    public:
        // Piston Group class constructor
        PistonGroup(const std::vector<pros::ADIDigitalOut> pistonsVec);

        // Piston Group class constructor
        PistonGroup(const std::vector<uint8_t> ports);
        PistonGroup(uint8_t port);

        void extend();
        void retract();
        void toggle();
        
        bool isExtended = false;

    private:
        std::vector<pros::ADIDigitalOut> pistonsVec;
};