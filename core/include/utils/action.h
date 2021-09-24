#ifndef _ACTION_
#define _ACTION_

#include "vex.h"
#include <functional>
#include "../core/include/subsystems/tank_drive.h"
using namespace vex;

/**
 * action_ptr is a std:function that returns int and takes no params
 * It was implemented this way because:
 * 1. We need to pass in lambdas, since lambdas can have captures,
 * which removes the need for function parameters (which would be a
 * headache if different functions required different parameters). 
 * 2. Plain function pointers have issues when you try to pass in a 
 * lambda that has captures.
 */
typedef std::function<int()> action_ptr;

class Action {
  protected:
    action_ptr myFnPtr;
    bool running;
    Action();

  public:
    Action(action_ptr fnPtr);

    int run();
    void stop();
};

class DriveAction:Action {
  private:
    DriveAction(TankDrive td);
    DriveAction(TankDrive td, directionType dir, double inches, double pct_speed);
    DriveAction(TankDrive td, double degrees, double pct_speed);

  public:
    static DriveAction stop_drive(TankDrive td);
    static DriveAction drive_forward(TankDrive td, double dist, double pct_speed);
    static DriveAction drive_reverse(TankDrive td, double dist, double pct_speed);
    static DriveAction turn_clockwise(TankDrive td, double degrees, double pct_speed);
    static DriveAction turn_counterclockwise(TankDrive td, double degrees, double pct_speed);
};

#endif