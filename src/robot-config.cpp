#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

brain  Brain;
controller main_controller;

// ======== OUTPUTS ========

// Drivetrain Components
motor lf_drive(PORT11, true), lr_drive(PORT12, true), rf_drive(PORT20), rr_drive(PORT19);

motor_group left_motors = {lf_drive, lr_drive};
motor_group right_motors = {rf_drive, rr_drive};

// Subsystems Components

// ======== INPUTS ========
inertial imu(PORT1);
vex::distance goal_sense(PORT10);
encoder left_enc(Brain.ThreeWirePort.C);
encoder right_enc(Brain.ThreeWirePort.E);

// ======== SUBSYSTEMS ========

// General Robot Configuration
robot_specs_t robot_cfg = {
  .robot_radius = 12, // inches
  .odom_wheel_diam = 2.84,//4.25, // inches
  .odom_gear_ratio = 1.03,//2.333333, // inches
  .dist_between_wheels = 9.18, // inches
  .drive_correction_cutoff = 12, //inches
  .drive_pid = (PID::pid_config_t) 
  {
    .p = .1,
    .i = .001,
    .d = .008,
    .f = 0,
    .k = .05,
    .deadband = 0.3,
    .on_target_time = 0
  },
  .turn_pid = (PID::pid_config_t)
  {
    .p = 0.025,
    .i = 0.01,
    .d = 0.0015,
    // .k = .05,
    .f = 0,
    .deadband = 5,
    .on_target_time = 0.1
  },
  .correction_pid = (PID::pid_config_t)
  {
    .p = .03,
    // .d = 0.002
  }
};

// Drivetrain
OdometryTank odom(left_enc, right_enc, robot_cfg, &imu);
TankDrive drive(left_motors, right_motors, robot_cfg, &odom);

// ======== UTILS ========


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // imu.calibrate();
}