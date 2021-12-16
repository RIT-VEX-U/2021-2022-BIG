#include "subsystems/lift.h"

Lift::Lift(vex::motor &lift_motor, vex::pneumatics &lift_claw)
: lift_motor(lift_motor), lift_claw(lift_claw)
{

}

void Lift::control(bool btn_lift_up, bool btn_lift_down, bool btn_claw_open, bool btn_claw_close)
{
  double pos = lift_motor.position(vex::rotationUnits::rev);

  // ======== LIFT MOTOR CONTROLS ========
  if(pos < LIFT_MIN || pos > LIFT_MAX)
    lift_motor.spinTo(pos, vex::rotationUnits::rev);    
  else if(btn_lift_up)
    lift_motor.spin(vex::directionType::fwd);
  else if(btn_lift_down)
    lift_motor.spin(vex::directionType::rev);
  else
    lift_motor.spinTo(pos, vex::rotationUnits::rev);
  
  // ======== LIFT CLAW CONTROLS ========
  if(btn_claw_open)
    lift_claw.open();
  else if(btn_claw_close)
    lift_claw.close();
  
}