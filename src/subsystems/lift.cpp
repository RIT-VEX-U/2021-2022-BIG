#include "subsystems/lift.h"

#define LIFT_DOWN 0
#define LIFT_DRIVE .5
#define LIFT_PLATFORM 1.5
#define LIFT_UP 1.5

#define LIFT_SPEED 2.0


Lift::Lift(vex::motor_group &lift_motors, vex::limit &lift_home, vex::pneumatics &lift_claw, PID::pid_config_t &lift_pid_cfg)
: lift_motors(lift_motors), lift_home(lift_home), lift_claw(lift_claw), lift_pid(lift_pid_cfg)
{
  hold = true;

  task bg([](void* ptr){
    Lift &lift = *((Lift*)ptr);

    while(true)
    {
      if(lift.get_bg_hold())
        lift.hold_lift();

      vexDelay(20);
    }

    return 0;
  }, this);
}

/**
  * Contains driver controls for the lift / claw subsystem.
  * Pressing up_btn will cycle the lift "up" through the positions, and down_btn does the opposite.
  * Positions include: "Down" (waiting for pickup), "Driving" (goal is kept just off the ground),
  *                    "Platform" (lift is placing on the platform), "UP" (max height)
  */
void Lift::control(bool up_btn, bool down_btn, bool claw_btn)
{
  
  static bool claw_btn_last = false;
  static bool claw_state = false;

  static timer ctl_tmr;

  // bool up_new_press = (up_btn && !up_btn_last);
  // bool down_new_press = (down_btn && !down_btn_last);
  bool claw_new_press = (claw_btn && !claw_btn_last);

  double pos = lift_motors.position(rotationUnits::rev);

  double min = (is_ring_collecting) ? LIFT_DRIVE : LIFT_DOWN ;

  if(up_btn && pos < LIFT_UP)
  {
    // lift_setpt += LIFT_SPEED * ctl_tmr.time(timeUnits::sec);
    setpoint = pos + .3;
    lift_motors.spin(directionType::fwd, 12, voltageUnits::volt);
    hold = false;
  } else if (down_btn && pos > min)
  {
    setpoint -= LIFT_SPEED * ctl_tmr.time(timeUnits::sec);
    hold = true;
  } else if (pos < min)
  {
    setpoint = min;
    hold = true;
  }else
  {
    hold = true;
  }

  ctl_tmr.reset();
  
  /*

  // Lift program control through a state machine
  switch(current_lift_pos)
  {
    case DOWN:
      if(up_new_press || is_ring_collecting)
        current_lift_pos = DRIVING;

    break;
    case DRIVING:
      if(up_new_press)
        current_lift_pos = UP;
      if(down_new_press)
        current_lift_pos = DOWN;

    break;
    // case PLATFORM:
    //   if(up_new_press)
    //     current_lift_pos = UP;
    //   if(down_new_press)
    //     current_lift_pos = DRIVING;

    // break;
    case UP:
      if(down_new_press)
        current_lift_pos = DRIVING;

    break;
    default:
    break;
  }

  set_lift_height(current_lift_pos);
  */

  // Toggle the claw and reset the lift integral term to avoid snap-back
  if(claw_new_press)
  {
    lift_pid.reset();
    claw_state = !claw_state;
    lift_claw.set(claw_state);
  }

  claw_btn_last = claw_btn;
}

/**
  * @deprecated
  * Old lift controls used for the december scrimmage. Kept for posterity.
  */
void Lift::manual_control(bool btn_lift_up, bool btn_lift_down, bool btn_claw_open, bool btn_claw_close)
{
  double pos = lift_motors.position(vex::rotationUnits::rev);

  // ======== LIFT MOTOR CONTROLS ========
  if(pos < LIFT_DOWN || pos > LIFT_UP)
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

/**
  * Automatically set the lift to predetermined heights and hold them there.
  * Values can be changed in the defines above.
  */
bool Lift::set_lift_height(LiftPosition pos)
{
  switch(pos)
  {
    case DOWN:
      setpoint = LIFT_DOWN;
    break;
    case DRIVING:
      setpoint = LIFT_DRIVE;
    break;
    case PLATFORM:
      setpoint = LIFT_PLATFORM;
    break;
    case UP:
      setpoint = LIFT_UP;
    break;
    default:
    break;
  }

  hold = true;

  return lift_pid.is_on_target();
}

/**
  * Using our custom PID loops, hold the lift at a certain height.
  */
void Lift::hold_lift(double rot)
{
  if(rot == __DBL_MAX__)
    rot=setpoint;

  lift_pid.set_target(rot);
  lift_pid.update(lift_motors.rotation(rotationUnits::rev));
  
  lift_motors.spin(directionType::fwd, lift_pid.get(), voltageUnits::mV);
}

/**
  * Home the lift to a zero position, either non-blocking or blocking
  */
bool Lift::home(bool blocking)
{
  static timer watchdog;
  static bool is_init = false;

  if(!is_init)
  {
    watchdog.reset();
    hold = false;
    is_init = false;
  }

  do
  {
    // If the watchdog times out, stop stressing the motor and return
    if(lift_home || watchdog.time(timeUnits::sec) > 5)
    {
      lift_motors.setPosition(0, rotationUnits::rev);
      is_init = false;
      hold = true;
      setpoint = LIFT_DOWN;
      return true;
    }

    lift_motors.spin(directionType::rev, 50, velocityUnits::pct);

  } while(blocking);

  return false;
}

void Lift::set_ring_collecting(bool val)
{
  is_ring_collecting = val;
}

bool Lift::get_bg_hold()
{
  return hold;
}