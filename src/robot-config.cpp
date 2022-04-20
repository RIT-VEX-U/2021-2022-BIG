#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

brain  Brain;
controller main_controller;

// ======== OUTPUTS ========

// Drivetrain Components
motor rt_drive(PORT14, gearSetting::ratio6_1), rf_drive(PORT13, gearSetting::ratio6_1, true), rm_drive(PORT12, gearSetting::ratio6_1, true), rb_drive(PORT11, gearSetting::ratio6_1, true), 
      lt_drive(PORT19, gearSetting::ratio6_1, true), lf_drive(PORT18, gearSetting::ratio6_1), lm_drive(PORT17, gearSetting::ratio6_1), lb_drive(PORT16, gearSetting::ratio6_1);

motor_group left_motors = {lt_drive, lf_drive, lm_drive, lb_drive};
motor_group right_motors = {rt_drive, rf_drive, rm_drive, rb_drive};

// Subsystems Components
motor l_feed(PORT9, true), r_feed(PORT8), l_lift(PORT6, true), r_lift(PORT7);
motor_group lift_motors = {l_lift, r_lift};

pneumatics front_solenoid(Brain.ThreeWirePort.H);
pneumatics rear_solenoid(Brain.ThreeWirePort.F);


// ======== INPUTS ========
inertial imu(PORT15);
vex::distance dist(PORT1);
CustomEncoder tracking_enc(Brain.ThreeWirePort.A, 2048);

// ======== SUBSYSTEMS ========

// General Robot Configuration
robot_specs_t robot_cfg = {
  .robot_radius = 12, // inches
  .odom_wheel_diam = 2.85,//4.25, // inches
  .odom_gear_ratio = 1.03,//2.333333, // inches
  .dist_between_wheels = 9.75, // inches
  .drive_correction_cutoff = 12, //inches
  .drive_pid = (PID::pid_config_t) 
  {
    .p = .2,
    .i = .1,
    .d = .02,
    .f = 0,
    .deadband = .5,
    .on_target_time = .1
  },
  .turn_pid = (PID::pid_config_t)
  {
    .p = 0.06,
    .i = 0.05,
    .d = 0.004,
    .f = 0,
    .deadband = 2,
    .on_target_time = 0.1
  },
  .correction_pid = (PID::pid_config_t)
  {
    .p = 0.02,
    .d = 0.0015
  }
};

// Drivetrain
OdometryTank odom(tracking_enc, tracking_enc, robot_cfg, &imu);
TankDrive drive(left_motors, right_motors, robot_cfg, &odom);

// Lift
Lift<LiftPosition>::lift_cfg_t lift_cfg = {
  .up_speed = 12, //volts
  .down_speed = 12, //volts / second
  .softstop_up = 1.9,
  .softstop_down = 0,
  .lift_pid_cfg =
  {
    .p = 100,
    .i = 0,
    .d = 3,
    .f = 0,

    .deadband = .1,
    .on_target_time = .1
  }
};

map<LiftPosition, double> lift_map {
  {DOWN, 0},
  {DRIVING, 0.6},
  {PLATFORM, 1.5},
  {UP, 1.9}
};

Lift<LiftPosition> lift_subsys(lift_motors, lift_cfg, lift_map);

// ======== UTILS ========
AutoChooser auto_chooser(Brain);


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  imu.calibrate();
}