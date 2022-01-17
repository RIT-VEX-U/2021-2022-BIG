#pragma once

#include "vex.h"
#include "core.h"

class Lift
{
  public:
  enum LiftPosition
  {
    DOWN, DRIVING, PLATFORM, UP
  };
  
  Lift(vex::motor_group &lift_motors, vex::limit &lift_home, vex::pneumatics &lift_claw, PID::pid_config_t &lift_pid_cfg);

  void control(bool up_btn, bool down_btn, bool claw_btn);

  void manual_control(bool btn_lift_up, bool btn_lift_down, bool btn_claw_open, bool btn_claw_close);

  bool set_lift_height(LiftPosition pos);

  void hold_lift(double rot);

  bool home(bool blocking=true);

  void set_ring_collecting(bool val);

  private:

  LiftPosition current_lift_pos = DOWN;

  vex::motor_group &lift_motors;
  vex::limit &lift_home;
  vex::pneumatics &lift_claw;
  PID lift_pid;

  bool is_ring_collecting = false;
};