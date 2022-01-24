#include "subsystems/ring_collector.h"

#define FORK_UP 0
#define FORK_DOWN 2.7
#define FORK_DRIVING 1.8
#define FORK_LOADING 1.05

#define CONV_MAX_CURRENT 1
#define CONV_JAM_TIME .5

RingCollector::RingCollector(vex::motor &fork, vex::motor &conveyor, vex::optical &goal_sensor, Lift &lift_subsys, PID::pid_config_t &fork_pid_cfg)
: fork(fork), conveyor(conveyor), goal_sensor(goal_sensor), lift_subsys(lift_subsys), fork_pid(fork_pid_cfg)
{
  hold_thread = true;

  task t([](void* ptr){
    RingCollector &ring_subsys = *((RingCollector*) ptr);

    while(true)
    {
      if(ring_subsys.get_hold_thread())
        ring_subsys.hold();
      
      vexDelay(20);
    }

    return 0;
  }, this);
}

/**
  * Control overview:
  */
void RingCollector::control(bool btn_lower_fork, bool btn_toggle_collect)
{
  static bool btn_toggle_collect_last = btn_toggle_collect;
  static bool btn_lower_fork_last = btn_lower_fork;
  static timer tmr;
  static timer jam_tmr;

  bool collect_toggle_new_press = btn_toggle_collect && !btn_toggle_collect_last;
  bool lower_fork_new_press = btn_lower_fork && !btn_lower_fork_last;

  // ======== FORK CONTROLS =======
  
  switch(curr_position)
  {
    case UP:
      
      if(lower_fork_new_press)
        curr_position = DOWN;

      conveyor.stop();
    break;
    case DOWN:

      if(lower_fork_new_press)
      {
        if(goal_sensor.isNearObject())
          curr_position = DRIVING;
        else
          curr_position = UP;
      }

      // if(goal_sensor.isNearObject() && !btn_lower_fork)
      // {
      //   tmr.reset();
      //   curr_position = DOWN_COOLDOWN;
      // }

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
      if(lower_fork_new_press)
      {
        curr_position = DOWN;
        // tmr.reset();
        // curr_position = DRIVING_COOLDOWN;
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
      
      if(lower_fork_new_press)
        curr_position = DOWN;

      conveyor.spin(directionType::fwd, 100, percentUnits::pct);
      
      if(conveyor.current() > CONV_MAX_CURRENT)
      {
        if(jam_tmr.time(timeUnits::sec) > CONV_JAM_TIME)
        {
          curr_position = UNJAMING;
          jam_tmr.reset();
        }
      }else
      {
        jam_tmr.reset();
      }

    break;
    case UNJAMING:
      if (jam_tmr.time(sec) > CONV_JAM_TIME)
        curr_position = LOADING;
      
      conveyor.spin(directionType::rev);
    break;
    default:
    break;
  }
  
  set_fork_pos(curr_position);

  // Make sure the lift is out of the way while collecting
  lift_subsys.set_ring_collecting(curr_position == LOADING);

  btn_toggle_collect_last = btn_toggle_collect;
  btn_lower_fork_last = btn_lower_fork;
}

bool RingCollector::set_fork_pos(ForkPosition pos)
{
  hold_thread = true;

  switch(pos)
  {
    case UP:
      fork_setpoint = FORK_UP;
    break;
    case DOWN:
    case DOWN_COOLDOWN:
    case DRIVING_COOLDOWN:
      fork_setpoint = FORK_DOWN;
    break;
    case DRIVING:
      fork_setpoint = FORK_DRIVING;
    break;
    case LOADING:
      fork_setpoint = FORK_LOADING;
    break;
    default:
    break;
  }

  return (fork_pid.get_target() == fork_setpoint) && fork_pid.is_on_target();
}

void RingCollector::hold(double pos)
{
  if(pos == __DBL_MAX__)
    pos = fork_setpoint;

  fork_pid.set_target(pos);
  fork_pid.update(fork.rotation(rotationUnits::rev));

  fork.spin(directionType::fwd, fork_pid.get(), voltageUnits::mV);
}

void RingCollector::home()
{
  static timer tmr;
  tmr.reset();

  while(fork.current() < 2.0 && tmr.time(sec) < 3)
  {
    fork.spin(directionType::rev, 12, voltageUnits::volt);
  }

  fork.setPosition(0, rotationUnits::rev);
}

bool RingCollector::get_hold_thread()
{
  return hold_thread;
}