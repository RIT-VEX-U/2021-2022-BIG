#pragma once

#include "vex.h"

#define LIFT_MIN 0
#define LIFT_MAX 10

class Lift
{
  public:
  Lift(vex::motor &lift_motor, vex::pneumatics &lift_claw);

  void control(bool btn_lift_up, bool btn_lift_down, bool btn_claw_open, bool btn_claw_close);

  private:

  vex::motor &lift_motor;
  vex::pneumatics &lift_claw;

};