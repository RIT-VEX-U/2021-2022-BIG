#pragma once

#include "core.h"
#include "subsystems/lift.h"
#include "subsystems/ring_collector.h"

using namespace vex;

extern brain Brain;

extern controller main_controller;

extern motor l_drive_top, l_drive_front, l_drive_mid, l_drive_back, 
             r_drive_top, r_drive_front, r_drive_mid, r_drive_back;

extern motor conveyor_motor, lift_motor, fork_motor;
extern pneumatics claw_solenoid;

extern motor_group left_motors, right_motors;

extern Lift lift_subsys;
extern RingCollector ring_subsys;

// extern encoder left_enc, right_enc;

extern TankDrive drive;

// extern OdometryTank odom;

// extern inertial imu;



// VEXcode devices

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );