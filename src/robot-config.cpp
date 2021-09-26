#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

motor lf(PORT2, gearSetting::ratio6_1, true), lr(PORT7, gearSetting::ratio6_1, true),
      rf(PORT1, gearSetting::ratio6_1, false), rr(PORT6, gearSetting::ratio6_1, false);

motor_group left_motors = {lf, lr};
motor_group right_motors = {rf, rr};

inertial imu(PORT5);

TankDrive::tankdrive_config_t tank_cfg = 
{
  .drive_pid = (PID::pid_config_t) 
  {
    .p = .15,
    .i = 0,
    .d = .01,
    .f = 0,
    .deadband = 0.1,
    .on_target_time = 0.1
  },

  .turn_pid = (PID::pid_config_t)
  {
    // Oscillation pt: 0.045
    // Ku = 0.06
    // Tu = .52
    .p = 0.025,
    .i = 0.001,
    .d = 0.0015,
    .f = 0,
    .deadband = 1,
    .on_target_time = 0.1
  },
  
  .correction_pid = (PID::pid_config_t)
  {
    .p = 0.01
  }
};


odometry_config_t odom_cfg = 
{
  .wheel_diam = 4.125,
  .gear_ratio = 2.333333
};

OdometryTank odom(left_motors, right_motors, imu, odom_cfg);

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