#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;

extern controller main_controller;

extern motor lf, lr, lr2, rf, rr, rr2;

extern motor_group left_motors, right_motors;

extern TankDrive::tankdrive_config_t tank_cfg;

extern TankDrive drive;

extern odometry_config_t odom_cfg;

extern OdometryTank odom;

extern inertial imu;



// VEXcode devices

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );