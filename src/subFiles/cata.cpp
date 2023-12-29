#include "subHeads/cata.hpp"

Cata::Cata(pros::MotorGroup* motorGroup) {
    motors = std::unique_ptr<pros::MotorGroup>(motorGroup);
    motors->set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    motors->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    motors->tare_position();
}

void Cata::resetPos() {
    motors->tare_position();
    angle = 0;
}

int Cata::getAngle() {
    return angle;
}

void Cata::update() {
    angle = (int)floor((*motors)[0].get_position() * CATA_RATIO) % 120;
}

void Cata::start(int power) {
    motors->move(power);
}

void Cata::stop() {
    motors->brake();
}

void Cata::standby() {
    int tolerance = 3;

    // ready to shoot logic
    if (abs(angle) <= tolerance) {
        stop();
    }
    std::cout << "encoder ticks: " << floor(std::remainder(motors->get_positions()[0] * CATA_RATIO, 120)) << ' ' << motors->get_positions()[0] << '\n';
}
