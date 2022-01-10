#include "subsystems/lift.h"


Lift::Lift(vex::motor_group &lift_motors, vex::pneumatics &lift_claw)
: lift_motors(lift_motors), lift_claw(lift_claw)
{

}



void Lift::claw_control(bool up_btn, bool down_btn)
{
  // if(down_btn && )
}

void Lift::manual_control(bool btn_lift_up, bool btn_lift_down, bool btn_claw_open, bool btn_claw_close)
{
  double pos = lift_motors.position(vex::rotationUnits::rev);

  // ======== LIFT MOTOR CONTROLS ========
  if(pos < LIFT_MIN || pos > LIFT_MAX)
    lift_motors.spinTo(pos, vex::rotationUnits::rev);    
  else if(btn_lift_up)
    lift_motors.spin(vex::directionType::fwd);
  else if(btn_lift_down)
    lift_motors.spin(vex::directionType::rev);
  else
    lift_motors.spinTo(pos, vex::rotationUnits::rev);
  
  // ======== LIFT CLAW CONTROLS ========
  if(btn_claw_open)
    lift_claw.open();
  else if(btn_claw_close)
    lift_claw.close();
  
}