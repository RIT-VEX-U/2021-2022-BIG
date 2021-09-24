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

/**
 * The Action superclass. Meant to be extended to make purpose-built
 * reusable actions, or construct an Action directly for a one-off
 * action.
 */
class Action {
  protected:
    action_ptr myFnPtr;
    bool running;
    Action();

  public:
    Action(action_ptr fnPtr);

    int run();
    bool isRunning();
    void stop();
};

/**
 * The DriveAction class. Extends Action to be an Action Factory.
 */
class DriveAction:public Action {
  private:
    TankDrive &td;
    DriveAction(TankDrive &td);
    DriveAction(TankDrive &td, directionType dir, double inches, double pct_speed);
    DriveAction(TankDrive &td, double degrees, double pct_speed);

  public:
    void stop();
    static DriveAction drive_forward(TankDrive &td, double dist, double pct_speed);
    static DriveAction drive_reverse(TankDrive &td, double dist, double pct_speed);
    static DriveAction turn_cw(TankDrive &td, double degrees, double pct_speed);
    static DriveAction turn_ccw(TankDrive &td, double degrees, double pct_speed);
};

#endif