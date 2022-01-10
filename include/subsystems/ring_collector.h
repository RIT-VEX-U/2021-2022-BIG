#pragma once

#include "vex.h"

#define FORK_UP 0.0
#define FORK_DOWN -3.0

class RingCollector
{
  public:

  RingCollector(vex::motor &fork, vex::motor &conveyor);
  
  void control(bool btn_toggle_fork, bool btn_collect);

  private:

  vex::motor &fork, &conveyor;

};