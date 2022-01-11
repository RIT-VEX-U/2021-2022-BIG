#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

motor l_drive_top(PORT11, true), l_drive_front(PORT12), l_drive_mid(PORT13), l_drive_back(PORT14, true), 
      r_drive_top(PORT20), r_drive_front(PORT19, true), r_drive_mid(PORT18, true), r_drive_back(PORT17);

motor_group left_motors = {l_drive_top, l_drive_front, l_drive_mid, l_drive_back};
motor_group right_motors = {r_drive_top, r_drive_front, r_drive_mid, r_drive_back};

motor conveyor_motor(PORT4, true), l_lift_motor(PORT5, true), r_lift_motor(PORT6), fork_motor(PORT7);

motor_group lift_motors = {l_lift_motor, r_lift_motor};

pneumatics claw_solenoid(Brain.ThreeWirePort.A);

limit lift_home(Brain.ThreeWirePort.B);

optical goal_sensor(PORT15);

// encoder left_enc(Brain.ThreeWirePort.A), right_enc(Brain.ThreeWirePort.C);

// inertial imu(PORT19);

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

PID::pid_config_t lift_pid_cfg = {
  .p = 40000,
  .i = 0,
  .d = 1000,
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

// OdometryTank odom(left_motors, right_motors, robot_cfg);

TankDrive drive(left_motors, right_motors, robot_cfg);


Lift lift_subsys(lift_motors, lift_home, claw_solenoid, lift_pid_cfg);
RingCollector ring_subsys(fork_motor, conveyor_motor, goal_sensor, lift_subsys, fork_pid_cfg);

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