#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

motor lf(PORT2, gearSetting::ratio6_1, true), lr(PORT11, gearSetting::ratio6_1, true),
      rf(PORT10, gearSetting::ratio6_1, false), rr(PORT20, gearSetting::ratio6_1, false);

motor_group left_motors = {lf, lr};
motor_group right_motors = {rf, rr};

encoder left_enc(Brain.ThreeWirePort.A), right_enc(Brain.ThreeWirePort.C);

inertial imu(PORT19);

robot_specs_t robot_cfg = {
  .robot_radius = 9, // inches
  .odom_wheel_diam = 2.85,//4.25, // inches
  .odom_gear_ratio = 1,//2.333333, // inches
  .dist_between_wheels = 9.75, // inches

  .drive_correction_cutoff = 3, //inches

  .drive_pid = (PID::pid_config_t) 
  {
    .p = .3,
    // .i = .00,
    .d = .03,
    .f = 0,
    .deadband = 0.1,
    .on_target_time = 0.1
  },

  .turn_pid = (PID::pid_config_t)
  {
    .p = 0.03,
    .i = 0.0,
    .d = 0.0015,
    .f = 0,
    .deadband = 2,
    .on_target_time = 0.1
  },

  .correction_pid = (PID::pid_config_t)
  {
    .p = 0.01
  }

};

OdometryTank odom(left_enc, right_enc, robot_cfg, &imu);

TankDrive drive(left_motors, right_motors, robot_cfg, &odom);

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