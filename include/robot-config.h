#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;
extern controller main_controller;

// ======== OUTPUTS ========

// Drivetrain Components
extern motor ldtf, ldtr, ldbf, ldbr, 
             rdtf, rdtr, rdbf, rdbr;

extern motor_group left_motors, right_motors, lift_motors;

// Subsystems Components
extern motor conveyor_motor, l_lift_motor, r_lift_motor, fork_motor;

extern pneumatics claw_solenoid, rear_clamp, flaps_solenoid;

// ======== INPUTS ========
extern inertial imu;
extern vex::distance goal_sense;
extern CustomEncoder left_enc, right_enc;

// ======== SUBSYSTEMS ========

// Drivetrain
extern TankDrive drive;
extern OdometryTank odom;

// Lift
enum LiftPosition {DOWN, DRIVING, PLATFORM, UP};
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