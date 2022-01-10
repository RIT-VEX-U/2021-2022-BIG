#pragma once

#include "vex.h"
#include "core.h"

#define LIFT_MIN 0
#define LIFT_MAX 1

class Lift
{
  public:
  Lift(vex::motor_group &lift_motors, vex::pneumatics &lift_claw);

  void claw_control(bool up_btn, bool down_btn);

  void manual_control(bool btn_lift_up, bool btn_lift_down, bool btn_claw_open, bool btn_claw_close);

  private:

  vex::motor_group &lift_motors;
  vex::pneumatics &lift_claw;

};