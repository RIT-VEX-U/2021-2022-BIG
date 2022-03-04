#pragma once
#include "robot-config.h"

namespace automation
{
  bool drive_to_goal(double speed, bool (*end_condition) (void));
}