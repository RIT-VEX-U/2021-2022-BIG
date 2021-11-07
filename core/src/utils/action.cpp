/**
 * File: action.cpp
 * Desc:
 *    Actions are the function pointers that are executed by a Command.
 *    Each subsystem should have its own set of default Actions. For 
 *    example, DriveAction drive_forward at line 134.
 *
 *    Currently, Actions can not take any parameters. This should be taken
 *    into account when creating custom Actions.
 *
 * Usage:
 *    
 */

#include "vex.h"
#include "vex_task.h"
#include <functional>
#include "../core/include/utils/action.h"
#include "../core/include/subsystems/tank_drive.h"
using namespace vex;

  
// Action class

Action::Action() {
  Action::myFnPtr = NULL;
}

/**
  * Create an Action with an action_ptr.
  *
  * @param fnPtr action_ptr to run when Action::run() is called.
  */
Action::Action(action_ptr fnPtr) {
  Action::myFnPtr = fnPtr;  
}

/**
  * Run and get the result of the function pointed to by myFnPtr.
  */
int Action::run() {
  Action::running = true;
  int result = Action::myFnPtr();
  return result;
}

/**
  * Get whether this Action is currently running.
  */
bool Action::isRunning() {
  return Action::running;
}

/**
  * Stop this Action prematurely.
  */
void Action::stop() {
  Action::running = false;
}

// PrintAction Class

/**
  * Create a PrintAction
  *
  * @param str string instance
  */
PrintAction::PrintAction(std::string &str):str(str) {
    myFnPtr = [&]() {
        printf(str.c_str());
        this->running = false;
        return true;
    };
}

void PrintAction::stop() {
    this->running = false;
}

PrintAction PrintAction::print_str(std::string &str) {
    printf("action.cpp: Creating print string action for %s\n", str.c_str());
    return PrintAction(str);
}

// DriveAction Class

/**
  * Create a DriveAction for linear fwd/rev movement in inches.
  *
  * @param td TankDrive instance
  * @param dir directionType direction of movement
  * @param inches double distance to move
  * @param pct_speed double percent (0.0-1.0) of power to apply
  */
DriveAction::DriveAction(TankDrive &td, directionType dir, double inches, double pct_speed):td(td) {
  printf("action.cpp: DriveAction::DriveAction(td, %i, %f, %f)\n", dir, inches, pct_speed);
  if (dir == directionType::rev) {
    inches = -inches;
  }
  Action::myFnPtr = [this, &td, inches, pct_speed]() {
    while (!td.drive_forward(inches, pct_speed)) {
    //while(!td.drive_to_point(0, inches, pct_speed, pct_speed)){
      task::sleep(20);
    }
    Action::running = false;
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
  printf("action.cpp: DriveAction::DriveAction(td, %f, %f)\n", degrees, pct_speed);
  Action::myFnPtr = [this, &td, degrees, pct_speed]() {
    while(!td.turn_degrees(degrees, pct_speed)) {
    //while(!td.turn_to_heading(degrees-90, pct_speed)){
      task::sleep(20);
    }
    Action::running = false;
    return true;
  };
}

void DriveAction::stop() {
  Action::running = false;
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