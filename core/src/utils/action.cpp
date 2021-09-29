#include "vex.h"
#include "vex_task.h"
#include <functional>
#include "../core/include/utils/action.h"
#include "../core/include/subsystems/tank_drive.h"
using namespace vex;

  
// Action class

Action::Action() {
  myFnPtr = NULL;
}

/**
  * Create an Action with an action_ptr.
  *
  * @param fnPtr action_ptr to run when Action::run() is called.
  */
Action::Action(action_ptr fnPtr) {
  this->myFnPtr = fnPtr;  
}

/**
  * Run and get the result of the function pointed to by myFnPtr.
  */
int Action::run() {
  this->running = true;
  int result = this->myFnPtr();
  return result;
}

/**
  * Get whether this Action is currently running.
  */
bool Action::isRunning() {
  return this->running;
}

/**
  * Stop this Action prematurely.
  */
void Action::stop() {
  this->running = false;
}

// DriveAction Class

/**
  * Create a DriveAction to halt and reinitialize TankDrive.
  *
  * @param td TankDrive instance
  */
DriveAction::DriveAction(TankDrive &td):td(td) {
  myFnPtr = [&]() {
    td.stop();
    td.reset_auto();
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
DriveAction::DriveAction(TankDrive &td, directionType dir, double inches, double pct_speed):td(td) {
  if (dir == directionType::rev) {
    inches = -inches;
  }
  myFnPtr = [&]() {
    while (!td.drive_forward(inches, pct_speed)) {
      task::sleep(50);
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
DriveAction::DriveAction(TankDrive &td, double degrees, double pct_speed):td(td) {
  myFnPtr = [&]() {
    while(!td.turn_degrees(degrees, pct_speed) && this->running) {
      task::sleep(20);
    }
    this->running = false;
    return true;
  };
}

void DriveAction::stop() {
  this->running = false;
  this->td.stop();
}

/**
  * Drive forward with TankDrive a set distance at a defined speed.
  */
DriveAction DriveAction::drive_forward(TankDrive &td, double dist, double pct_speed) {
  printf("action.cpp: Creating drive forward action for %f inches at %f power\n", dist, pct_speed);
  return DriveAction(td, directionType::fwd, dist, pct_speed);
}
/**
  * Drive reverse with TankDrive a set distance at a defined speed.
  */
DriveAction DriveAction::drive_reverse(TankDrive &td, double dist, double pct_speed) {
  return DriveAction(td, directionType::rev, dist, pct_speed);
}
/**
  * Turn CW with TankDrive a set # of degrees at a defined speed.
  */
DriveAction DriveAction::turn_cw(TankDrive &td, double degrees, double pct_speed) {
  printf("action.cpp: Creating turn clockwise action for %f degrees at %f power\n", degrees, pct_speed);
  return DriveAction(td, degrees, pct_speed);
}
/**
  * Turn CCW with TankDrive a set # of degrees at a defined speed.
  */
DriveAction DriveAction::turn_ccw(TankDrive &td, double degrees, double pct_speed) {
  return DriveAction(td, -degrees, pct_speed);
}