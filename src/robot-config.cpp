#include "vex.h"

#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;
controller controller1;

//Chassis (top, front, middle, back)
motor lt(PORT11, gearSetting::ratio6_1, true), lf(PORT12, gearSetting::ratio6_1, true),
      lm(PORT13, gearSetting::ratio6_1, true), lb(PORT14, gearSetting::ratio6_1, true),
      rt(PORT20, gearSetting::ratio6_1, true), rf(PORT19, gearSetting::ratio6_1, true),
      rm(PORT18, gearSetting::ratio6_1, true), rb(PORT17, gearSetting::ratio6_1, true);

//Lift
motor lLift(PORT5, gearSetting::ratio36_1, true), rLift(PORT6, gearSetting::ratio36_1, true);

//Feed, fork
motor feed(PORT4, gearSetting::ratio18_1, true), fork(PORT10, gearSetting::ratio36_1, true);

//Sensors
inertial imu(PORT19);
bumper liftBtn(Brain.ThreeWirePort.A);
limit forkSwitch(Brain.ThreeWirePort.B);
pneumatics claw(Brain.ThreeWirePort.C);
//encoder left_enc(Brain.ThreeWirePort.D), right_enc(Brain.ThreeWirePort.E);

motor_group left_motors = {lt,lf,lm,lb};
motor_group right_motors = {rt,rf,rm,rb};
motor_group fourbar = {lLift, rLift};

robot_specs_t robot_cfg = {
  .robot_radius = 9, // inches
  .odom_wheel_diam = 2.85, // inches
  .odom_gear_ratio = 1, // inches
  .dist_between_wheels = 9.75, // inches EDIT

  .drive_correction_cutoff = 3, //inches (when the correction PID disengages from target)

  .drive_pid = (PID::pid_config_t) 
  {
    .p = .3,
    .i = .00,
    .d = .03,
    .f = 0,
    .deadband = 0.1, //cut off from target threshold 
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

  .lift_pid = (PID::pid_config_t){
    .p = 1;
  }

};

//OdometryTank odom(left_enc, right_enc, robot_cfg, &imu);
OdometryTank odom(left_motors, right_motors, robot_cfg, &imu);
TankDrive drive(left_motors, right_motors, robot_cfg, &odom);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}