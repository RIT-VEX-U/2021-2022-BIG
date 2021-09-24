#include "vex.h"
#include "vex_task.h"
#include <functional>
#include "../core/include/utils/action.h"
#include "../core/include/subsystems/tank_drive.h"
using namespace vex;

/**
 * The Action superclass. Meant to be extended to make purpose-built
 * reusable actions, or construct an Action directly for a one-off
 * action.
 */
class Action::Action {
  protected:
    action_ptr myFnPtr;
    bool running;
    Action() {} //Default Constructor so we can extend with no issue

  public:
    /**
     * Create an Action with an action_ptr.
     *
     * @param fnPtr action_ptr to run when Action::run() is called.
     */
    Action(action_ptr fnPtr) {
      this->myFnPtr = fnPtr;  
    }

    /**
     * Run and get the result of the function pointed to by myFnPtr.
     */
    int run() {
      this->running = true;
      return this->myFnPtr();
    }

    /**
     * Get whether this Action is currently running.
     */
    bool isRunning() {
      return this->running;
    }

    /**
     * Stop this Action prematurely.
     */
    void stop() {
      this->running = false;
    }
};

/**
 * The DriveAction class. Extends Action to be an Action Factory.
 */
class DriveAction::DriveAction:Action {
    /**
     * Create a DriveAction to halt and reinitialize TankDrive.
     *
     * @param td TankDrive instance
     */
    DriveAction(TankDrive td) {
      myFnPtr = [&]() {
        td.stop();
        this->running = false;
        return true;
      };
    }

    /**
     * Create a DriveAction for linear fwd/rev movement in inches.
     *
     * @param td TankDrive instance
     * @param dir directionType direction of movement
     * @param inches double distance to move
     * @param pct_speed double percent (0.0-1.0) of power to apply
     */
    DriveAction(TankDrive td, directionType dir, double inches, double pct_speed) {
      if (dir == directionType::rev) {
        inches = -inches;
      }
      myFnPtr = [&]() {
        while (td.drive_forward(inches, pct_speed) != true && this->running) {
          task::sleep(20);
        }
        this->running = false;
        return true;
      };
    }

    /**
     * Create a DriveAction for rotational movement in degrees.
     * 
     * @param td TankDrive instance
     * @param degrees double degrees to rotate (CW=+, CCW=-)
     * @param pct_speed double percent (0.0-1.0) of power to apply
     */
    DriveAction(TankDrive td, double degrees, double pct_speed) {
      myFnPtr = [&]() {
        while(td.turn_degrees(degrees, pct_speed) && this->running) {
          task::sleep(20);
        }
        this->running = false;
        return true;
      };
    }

  public:
    static DriveAction stop_drive(TankDrive td) {
      return DriveAction(td);
    }
    /**
     * Drive forward with TankDrive a set distance at a defined speed.
     */
    static DriveAction drive_forward(TankDrive td, double dist, double pct_speed) {
      return DriveAction(td, directionType::fwd, dist, pct_speed);
    }
    /**
     * Drive reverse with TankDrive a set distance at a defined speed.
     */
    static DriveAction drive_reverse(TankDrive td, double dist, double pct_speed) {
      return DriveAction(td, directionType::rev, dist, pct_speed);
    }
    /**
     * Turn CW with TankDrive a set # of degrees at a defined speed.
     */
    static DriveAction turn_cw(TankDrive td, double degrees, double pct_speed) {
      return DriveAction(td, degrees, pct_speed);
    }
    /**
     * Turn CCW with TankDrive a set # of degrees at a defined speed.
     */
    static DriveAction turn_ccw(TankDrive td, double degrees, double pct_speed) {
      return DriveAction(td, -degrees, pct_speed);
    }
};