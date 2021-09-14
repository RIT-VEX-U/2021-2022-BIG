#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

motor lf(PORT14, gearSetting::ratio6_1), lr(PORT13, gearSetting::ratio6_1, true), lr2(PORT2, gearSetting::ratio6_1, true),
      rf(PORT18, gearSetting::ratio6_1, true), rr(PORT17, gearSetting::ratio6_1), rr2(PORT9, gearSetting::ratio6_1);

motor_group left_motors = {lf, lr, lr2};
motor_group right_motors = {rf, rr, rr2};

inertial imu(PORT16);

TankDrive::tankdrive_config_t tank_cfg = 
{
  .drive_pid = (PID::pid_config_t) 
  {
    .p = .1,
    .i = 0,
    // .d = .004,
    .f = 0,
    .deadband = 0.1,
    .on_target_time = 0.1
  },

  .turn_pid = (PID::pid_config_t)
  {
    .p = 0.008,
    .i = 0,
    .d = 0.0001,
    .f = 0,
    .deadband = 1,
    .on_target_time = 0.1
  },

  .wheel_diam=3.25,
  .dist_between_wheels = 11.6
};

odometry_config_t odom_cfg = 
{
  .wheel_diam = 3.25,
  .dist_between_wheels = 11.6,
  .gear_ratio = 1.66667
};

OdometryTank odom(left_motors, right_motors, odom_cfg);

TankDrive drive(left_motors, right_motors, imu, tank_cfg, &odom);

controller main_controller;

// VEXcode device constructors

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}