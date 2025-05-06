#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"
extern Drive chassis;

inline pros::adi::DigitalOut clamp_piston('F');
inline pros::Motor intake_motor(19);
inline pros::Motor conveyor_motor(18);
inline pros::MotorGroup climber_motors({-9, 13});

inline pros::adi::DigitalOut climber_piston('A');
inline pros::adi::DigitalOut passiveHook_piston('B');


inline pros::Imu gatr_IMU(21);

// inline pros::Motor intake(5);
// 

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');