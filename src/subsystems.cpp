#include "subsystems.h"

bool front_claw::is_open = false;
bool rear_claw::is_open = false;

vex::task *conveyor::task_handle = NULL;
std::atomic<bool> conveyor::is_running(true);

// ======== Pneumatic claw controls (front and rear) ========

// Define the the opening behaviour of the claw.
// Return a boolean for compatibility with GenericAuto
bool front_claw::open()
{
  claw_solenoid.open();
  is_open = true;
  return true;
}

// Define the the closing behaviour of the claw.
// Return a boolean for compatibility with GenericAuto
bool front_claw::close()
{
  claw_solenoid.close();
  is_open = false;
  return true;
}

// Control the claw with a toggle button
void front_claw::control(bool toggle)
{
  static bool last_btn = toggle;
  bool new_press = !last_btn && toggle;
  last_btn = toggle;

  if(new_press)
  {
    if(is_open)
      close();
    else
      open();
  }
}

// Define the the opening behaviour of the back claw.
// Return a boolean for compatibility with GenericAuto
bool rear_claw::open()
{
  rear_clamp.open();
  is_open = true;
  return true;
}

// Define the the closing behaviour of the back claw.
// Return a boolean for compatibility with GenericAuto
bool rear_claw::close()
{
  rear_clamp.close();
  is_open = true;
  return true;
}

// Control the claw with a toggle button
void rear_claw::control(bool toggle)
{
  static bool last_btn = toggle;
  bool new_press = !last_btn && toggle;
  last_btn = toggle;

  if(new_press)
  {
    if(is_open)
      close();
    else
      open();
  }
}

// ======== Ring Conveyor Controls ========
void conveyor::initialize()
{
  if(task_handle != NULL)
    return;
    
  // Run the conveyor system in a separate task to generalize overcurrent protection for driver and auto
  conveyor::task_handle = new vex::task([]() -> int {
    static bool overcurrent = false;
    static timer tmr;
    while(true)
    {
      if(is_running)
      {
        // If the system detects a jam, spin the motor backwards to unjam it
        if(overcurrent)
          conveyor_motor.spin(directionType::rev, 12, volt);
        else
          conveyor_motor.spin(directionType::fwd, 12, volt);

        // If the system was not previously overcurrent, but is now, start the unjamming process.
        // Add a minimum of 1 second after the last overcurrent protection ran to avoid the "changing directions"
        // false alarm issue.
        if(!overcurrent && conveyor_motor.current() > 2 && tmr.time() > 1000)
        {
          overcurrent = true;
          tmr.reset();
        }

        // Reset the overcurrent status after 500 milliseconds
        if(overcurrent && tmr.time() > 500)
          overcurrent = false;

      } else // not if running
      {
        conveyor_motor.stop();
      }

      vexDelay(20);
    }
    return 0;
  });
}

// Begin running the conveyor.
// Return a boolean for compatibility with GenericAuto
bool conveyor::start()
{
  if(task_handle == NULL)
    initialize();
  is_running = true;

  return true;
}

// Stop running the conveyor.
// Return a boolean for compatibility with GenericAuto
bool conveyor::stop()
{
  if(task_handle == NULL)
    initialize();
  is_running = false;

  return true;
}

// Control the conveyor with a toggle button
void conveyor::control(bool toggle)
{
  if (task_handle == NULL)
    initialize();

  static bool last_btn = toggle;
  bool new_press = !last_btn && toggle;
  last_btn = toggle;

  if(new_press)
    is_running = !is_running;
}