#pragma once
#include "robot-config.h"

namespace automation
{
  enum GoalType {YELLOW, BLUE, RED, ANY};

  bool drive_to_goal(double speed, bool (*end_condition) (void), GoalType color=ANY, directionType dir=fwd);
  void drive_with_autoaim(double left, double right, int power, GoalType color=ANY);
}