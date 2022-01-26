#include "../core/include/subsystems/lift.h"

/**
* Construct lift object with given parameters
*/
lift::lift(double aLength,double gRatio, double sAngle, double bOffset, double sHeight){
    armLength = aLength;
    gearRatio = gRatio;
    startAngle = sAngle;
    offset = bOffset;
    pivotHeight = sHeight;
}

/**
* Returns the length of the arm in inches from ceter of pivot to pivot
*/
double lift::get_length(){
  return armLength;
}

/**
* Checks to see if lift has zero'd it's position
*/
bool lift::liftDown(){
  if(liftBtn.pressing()){
    fourbar.setPosition(lift.sAngle/lift.gearRatio, deg); //if button is being pressed, reset position
    return true;
  }
  return false;
}

/**
* Returns the hight 
*/
double lift::get_height(){
  double height = lift.startHeight - lift.offset - cos(lLift.position(deg*lift.gearRatio))*lift.length; //from kindematics -- the greater the angle, the higher the lift
  return height;
}

/**
* Moves the lift to a certain height from the ground in inches
*/
void lift::move_to_height(double height){
    lift_pid.set_target(height);

    while(!lift_pid.is_on_target()){
      lift_pid.update(lift.get_height()); //set target height
      fourbar.spin(fwd, lift_pid.get(), volt); //send power to lift

      wait(20, msec);
    }
    fourbar.setStopping(hold); //maintain height
}