#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;
extern controller main_controller;

// ======== OUTPUTS ========

// Drivetrain Components
extern motor lf_drive, lr_drive, rf_drive, rr_drive;

extern motor_group left_motors, right_motors;

// Subsystems Components

// ======== INPUTS ========
extern inertial imu;
extern vex::distance goal_sense;
extern encoder left_enc, right_enc;

// ======== SUBSYSTEMS ========

// Drivetrain
extern TankDrive drive;
extern OdometryTank odom;


// ======== UTILS ========
extern robot_specs_t robot_cfg;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );