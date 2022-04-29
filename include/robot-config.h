#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;
extern controller main_controller;

// ======== OUTPUTS ========

// Drivetrain Components
extern motor lt_drive, lf_drive, lm_drive, lb_drive,
             rt_drive, rf_drive, rm_drive, rb_drive;

extern motor_group left_motors, right_motors, lift_motors;

// Subsystems Components
extern motor l_feed, r_feed, l_lift, r_lift;

extern pneumatics front_solenoid, rear_solenoid;

// ======== INPUTS ========
extern inertial imu;
extern vex::distance dist;
extern CustomEncoder tracking_enc;
extern limit rear_switch;

// ======== SUBSYSTEMS ========

// Drivetrain
extern TankDrive drive;
extern OdometryTank odom;

// Lift
enum LiftPosition {DOWN, LOW, DRIVING, PLATFORM, UP};
extern std::map<LiftPosition, double> lift_map;
extern Lift<LiftPosition> lift_subsys;

// ======== UTILS ========
extern AutoChooser auto_chooser;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );