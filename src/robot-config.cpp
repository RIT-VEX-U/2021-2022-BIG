#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

motor ldtf(PORT19, gearSetting::ratio6_1, true), ldtr(PORT18, gearSetting::ratio6_1, true), ldbf(PORT20, gearSetting::ratio6_1), ldbr(PORT17, gearSetting::ratio6_1), 
      rdtf(PORT13, gearSetting::ratio6_1), rdtr(PORT15, gearSetting::ratio6_1), rdbf(PORT12, gearSetting::ratio6_1, true), rdbr(PORT14, gearSetting::ratio6_1, true);

motor_group left_motors = {ldtf, ldtr, ldbf, ldbr};
motor_group right_motors = {rdtf, rdtr, rdbf, rdbr};

motor conveyor_motor(PORT11, true), l_lift_motor(PORT1, true), r_lift_motor(PORT2);

motor_group lift_motors = {l_lift_motor, r_lift_motor};

pneumatics claw_solenoid(Brain.ThreeWirePort.B);

pneumatics rear_clamp(Brain.ThreeWirePort.H);

pneumatics flaps(Brain.ThreeWirePort.C);

// limit lift_home(Brain.ThreeWirePort.B);

// optical goal_sensor(PORT15);

// encoder left_enc(Brain.ThreeWirePort.A), right_enc(Brain.ThreeWirePort.C);

inertial imu(PORT16);

robot_specs_t robot_cfg = {
  .robot_radius = 12, // inches
  .odom_wheel_diam = 2.85,//4.25, // inches
  .odom_gear_ratio = 1.03,//2.333333, // inches
  .dist_between_wheels = 9.75, // inches

  .drive_correction_cutoff = 12, //inches

  .drive_pid = (PID::pid_config_t) 
  {
    .p = .5,
    // .i = .01,
    .d = .0015,
    .f = 0,
    .k = .1,
    .deadband = 0.3,
    .on_target_time = 0
  },

  .turn_pid = (PID::pid_config_t)
  {
    .p = 0.12,
    .i = 0.0,
    .d = 0.012,
    .f = 0,
    .deadband = 2,
    .on_target_time = 0.1
  },

  .correction_pid = (PID::pid_config_t)
  {
    .p = 0.02,
    .d = 0.002
  }

};

PID::pid_config_t lift_pid_cfg = {
  /* .p = 40000,*/ .p = 4,
  .i = 0,
  .d = 4000,
  .f = 0,

  .deadband = .1,
  .on_target_time = .1
};

PID::pid_config_t fork_pid_cfg = {
  .p = 50000,
  .i = 0,
  .d = 0,
  .f = 0,

  .deadband = .1,
  .on_target_time = .1
};

OdometryTank odom(left_motors, right_motors, robot_cfg, &imu);

TankDrive drive(left_motors, right_motors, robot_cfg, &odom);

Lift<LiftPosition>::lift_cfg_t lift_cfg = {
  .up_speed = 12, //volts
  .down_speed = 24, //volts / second
  .softstop_up = 2.1,
  .softstop_down = 0
};

map<LiftPosition, double> lift_map {
  {DOWN, 0},
  {DRIVING, 0.5},
  {PLATFORM, 1.5},
  {UP, 2.1}
};

Lift<LiftPosition> lift_subsys(lift_motors, lift_cfg, lift_map);
// RingCollector ring_subsys(fork_motor, conveyor_motor, goal_sensor, lift_subsys, fork_pid_cfg);

controller main_controller;

// VEXcode device constructors

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  imu.calibrate();
}