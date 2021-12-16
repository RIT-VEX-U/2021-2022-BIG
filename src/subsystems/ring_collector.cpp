#include "subsystems/ring_collector.h"

RingCollector::RingCollector(vex::motor &fork, vex::motor &conveyor)
: fork(fork), conveyor(conveyor)
{

}

/**
  * Control overview:
  * btn_toggle_fork - a button that toggles whether the fork is "up" or "down"
  * btn_collect - When pressed, the ring conveyor / intake will run.
  */
void RingCollector::control(bool btn_toggle_fork, bool btn_collect)
{

  // ======== FORK CONTROLS ========
  static bool is_fork_toggled = false;
  static bool fork_prev_pressing = btn_toggle_fork;
  
  if(is_fork_toggled)
    fork.spinTo(FORK_UP, vex::rotationUnits::rev, false);
  else
    fork.spinTo(FORK_DOWN, vex::rotationUnits::rev, false);

  // On the rising edge of the button press, toggle
  if(btn_toggle_fork && !fork_prev_pressing)
    is_fork_toggled = !is_fork_toggled;

  fork_prev_pressing = btn_toggle_fork;

  // ======== CONVEYOR CONTROLS ========
  if(btn_collect)
    conveyor.spin(vex::directionType::fwd);
  else
    conveyor.stop();

}