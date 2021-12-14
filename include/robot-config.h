#pragma once

#include "core.h"

using namespace vex;

extern brain Brain;
extern controller controller1;

extern motor lt,lf,lm,lb, rt,rf,rm,rb;
extern motor lLift, rLift;
extern motor fork, feed;

extern motor_group left_motors, right_motors;
extern motor_group fourbar;

extern inertial imu;
extern bumper liftBtn;
extern limit forkSwitch;
extern pneumatics claw;
//extern encoder left_enc, right_enc;

extern TankDrive drive;
extern OdometryTank odom;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );