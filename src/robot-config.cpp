#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

optical line_tracker(PORT9);
line lt(Brain.ThreeWirePort.A);

#ifdef NEMO
  motor lf(PORT2, gearSetting::ratio6_1, true), lr(PORT7, gearSetting::ratio6_1, true),
      rf(PORT1, gearSetting::ratio6_1, false), rr(PORT6, gearSetting::ratio6_1, false);

  motor_group left_motors = {lf, lr};
  motor_group right_motors = {rf, rr};
#else
  motor l1(PORT15), l2(PORT5), l3(PORT4), l4(PORT14, true), r1(PORT17, true), r2(PORT16), r3(PORT7), r4(PORT6);

  motor_group left_motors = {l1, l2, l3, l4};
  motor_group right_motors = {r1, r2, r3, r4};
#endif

inertial imu(PORT8);

TankDrive::tankdrive_config_t tank_cfg = 
{
  .drive_pid = (PID::pid_config_t) 
  {
    .p = .5,
    .i = .00,
    .d = .02,
    .f = 0,
    .deadband = 0.1,
    .on_target_time = 0.1
  },

  .turn_pid = (PID::pid_config_t)
  {
    // Oscillation pt: 0.045
    // Ku = 0.06
    // Tu = .52
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


odometry_config_t odom_cfg = 
{
  .wheel_diam = 4.25,
  .gear_ratio = 2.333333,
  .dist_between_wheels = 9.75
};

OdometryTank odom(left_motors, right_motors, imu, odom_cfg);

TankDrive drive(left_motors, right_motors, tank_cfg, &odom);

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