#pragma once

#include "vex.h"
#include "core.h"
#include "subsystems/lift.h"
#include <atomic>

class RingCollector
{
  public:
  
  enum ForkPosition
  {
    UP, DOWN, DOWN_COOLDOWN, DRIVING, DRIVING_COOLDOWN, LOADING, UNJAMING
  };

  RingCollector(vex::motor &fork, vex::motor &conveyor, vex::optical &goal_sensor, Lift &lift_subsys, PID::pid_config_t &fork_pid_config);
  
  void control(bool btn_toggle_fork, bool btn_toggle_collect);

  bool set_fork_pos(ForkPosition pos);

  void hold(double pos=__DBL_MAX__);

  void home();

  bool get_hold_thread();

  std::atomic<bool> hold_thread;

  private:

  ForkPosition curr_position = UP;

  vex::motor &fork, &conveyor;
  vex::optical &goal_sensor;
  Lift &lift_subsys;

  double fork_setpoint;

  PID fork_pid;

};