#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;

extern controller main_controller;

extern motor ldtf, ldtr, ldbf, ldbr, 
             rdtf, rdtr, rdbf, rdbr;

extern motor conveyor_motor, l_lift_motor, r_lift_motor, fork_motor;
extern pneumatics claw_solenoid;

extern pneumatics rear_clamp, flaps;

extern motor_group left_motors, right_motors, lift_motors;

// extern limit lift_home;

enum LiftPosition {DOWN, DRIVING, PLATFORM, UP};
extern std::map<LiftPosition, double> lift_map;
extern Lift<LiftPosition> lift_subsys;
// extern RingCollector ring_subsys;
// extern optical goal_sensor;

// extern encoder left_enc, right_enc;

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