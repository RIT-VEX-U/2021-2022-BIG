#pragma once

#include "vex.h"
#include "core.h"
#include "subsystems/lift.h"

class RingCollector
{
  public:
  
  enum ForkPosition
  {
    UP, DOWN, DOWN_COOLDOWN, DRIVING, DRIVING_COOLDOWN, LOADING
  };

  RingCollector(vex::motor &fork, vex::motor &conveyor, vex::optical &goal_sensor, Lift &lift_subsys, PID::pid_config_t &fork_pid_config);
  
  void control(bool btn_toggle_fork, bool btn_toggle_collect);

  bool set_fork_pos(ForkPosition pos);

  void hold(double pos);

  private:

  ForkPosition curr_position = UP;

  vex::motor &fork, &conveyor;
  vex::optical &goal_sensor;
  Lift &lift_subsys;

  PID fork_pid;

};