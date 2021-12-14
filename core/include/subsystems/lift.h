#ifndef _LIFT_
#define _LIFT_

#include "vex.h"
#include "robot-config.h"
using namespace vex;

class lift{
  double armLength, //in
      gearRatio, 
      startAngle, //deg
      offset, //the distance between the lower lift pivot point and bottom of clamp mogo support
      pivotHeight; //in (height of pivot of lower arm to the ground)

  public:
  //contructor
  lift(double, double, double, double, double);

  //functions
  double get_length();
  bool liftDown();
  double get_height();
  void move_to_height(double height);

  private:

  PID lift_pid;
};

#endif