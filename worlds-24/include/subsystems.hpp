#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"
extern Drive chassis;

inline pros::adi::DigitalOut clamp_piston('A');
inline pros::Motor intake_motor(19);
inline pros::Motor conveyor_motor(18);
inline pros::MotorGroup ladybrown_motors({-20, 16});
inline pros::Rotation ladybrown_encoder(15);



inline pros::adi::DigitalOut climber_piston('F');

// inline pros::Motor intake(5);
// 

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');