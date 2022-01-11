#include "subsystems/ring_collector.h"

#define FORK_UP 0
#define FORK_DOWN 2.5
#define FORK_DRIVING 1.8
#define FORK_LOADING 1.05

RingCollector::RingCollector(vex::motor &fork, vex::motor &conveyor, vex::optical &goal_sensor, Lift &lift_subsys, PID::pid_config_t &fork_pid_cfg)
: fork(fork), conveyor(conveyor), goal_sensor(goal_sensor), lift_subsys(lift_subsys), fork_pid(fork_pid_cfg)
{

}

/**
  * Control overview:
  */
void RingCollector::control(bool btn_lower_fork, bool btn_toggle_collect)
{
  static bool btn_toggle_collect_last = btn_toggle_collect;
  static timer tmr;

  bool collect_toggle_new_press = btn_toggle_collect && !btn_toggle_collect_last;


  // ======== FORK CONTROLS =======
  
  switch(curr_position)
  {
    case UP:
      curr_position = DOWN;

      conveyor.stop();
    break;
    case DOWN:

      if(goal_sensor.isNearObject() && !btn_lower_fork)
      {
        tmr.reset();
        curr_position = DOWN_COOLDOWN;
      }

      if(collect_toggle_new_press)
        curr_position = LOADING;

      conveyor.stop();

    break;
    case DOWN_COOLDOWN:
      if(tmr.time(timeUnits::msec) > 300)
        curr_position = DRIVING;

      conveyor.stop();

    break;
    case DRIVING:
      if(btn_lower_fork)
      {
        tmr.reset();
        curr_position = DRIVING_COOLDOWN;
      }

      if(collect_toggle_new_press)
        curr_position = LOADING;

      conveyor.stop();

    break;
    case DRIVING_COOLDOWN:
      if(tmr.time(timeUnits::msec) > 1000)
        curr_position = DOWN;

        conveyor.stop();

    break;
    case LOADING:

      if(collect_toggle_new_press)
        curr_position = DRIVING;
      
      if(btn_lower_fork)
        curr_position = DOWN_COOLDOWN;

      lift_subsys.set_lift_height(Lift::LiftPosition::DRIVING);
      conveyor.spin(directionType::fwd, 100, percentUnits::pct);

    break;
    default:
    break;
  }
  
  set_fork_pos(curr_position);

  btn_toggle_collect_last = btn_toggle_collect;
}

bool RingCollector::set_fork_pos(ForkPosition pos)
{
  switch(pos)
  {
    case UP:
      hold(FORK_UP);
    break;
    case DOWN:
    case DOWN_COOLDOWN:
    case DRIVING_COOLDOWN:
      hold(FORK_DOWN);
    break;
    case DRIVING:
      hold(FORK_DRIVING);
    break;
    case LOADING:
      hold(FORK_LOADING);
    break;
    default:
    break;
  }

  return fork_pid.is_on_target();
}

void RingCollector::hold(double pos)
{
  fork_pid.set_target(pos);
  fork_pid.update(fork.rotation(rotationUnits::rev));

  fork.spin(directionType::fwd, fork_pid.get(), voltageUnits::mV);
}