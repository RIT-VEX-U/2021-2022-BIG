#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;

extern controller main_controller;

extern motor lf, lr, rf, rr;

extern motor_group left_motors, right_motors;

extern encoder left_enc, right_enc;

extern TankDrive drive;

extern OdometryTank odom;

extern inertial imu;



// VEXcode devices

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );