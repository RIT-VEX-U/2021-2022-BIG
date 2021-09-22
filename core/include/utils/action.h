#ifndef _ACTION_
#define _ACTION_

#include "vex.h"
#include <functional>
#include "../core/include/subsystems/tank_drive.h"
using namespace vex;

typedef std::function<int()> action_ptr;
class Action {
  protected:
    action_ptr myFnPtr;
    Action();

  public:
    Action(action_ptr fnPtr);

    int run();
};

class DriveAction:Action {
  private:
    DriveAction(TankDrive td, directionType dir, double inches, double pct_speed);

  public:
    static DriveAction drive_forward(TankDrive td, double dist, double pct_speed);
    static DriveAction drive_reverse(TankDrive td, double dist, double pct_speed);
};

#endif