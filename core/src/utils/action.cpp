#include "vex.h"
#include <functional>
#include "../core/include/utils/action.h"
#include "../core/include/subsystems/tank_drive.h"
using namespace vex;

class Action::Action {
  protected:
    action_ptr myFnPtr;
    Action() {}

  public:
    Action(action_ptr fnPtr) {
      this->myFnPtr = fnPtr;  
    }

    int run() {
      return this->myFnPtr();
    }
};

class DriveAction::DriveAction:Action {
    DriveAction(TankDrive td, directionType dir, double inches, double pct_speed) {
      if (dir == directionType::rev) {
        inches = -inches;
      }
      myFnPtr = [&]() {
        td.drive_forward(inches, pct_speed);
        return 0;
      };
    }

  public:
    static DriveAction drive_forward(TankDrive td, double dist, double pct_speed) {
      return DriveAction(td, directionType::fwd, dist, pct_speed);
    }
    static DriveAction drive_reverse(TankDrive td, double dist, double pct_speed) {
      return DriveAction(td, directionType::rev, dist, pct_speed);
    }
};