#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"

/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  while(imu.isCalibrating());

  task homing_task([](){
    lift_subsys.home();
    lift_subsys.set_lift_height(Lift::LiftPosition::DOWN);
    return 0;
  });

  // ========== INIT ==========
  // Odometry set up
  odom.set_position({.x=21.7, .y=13.7, .rot=77.5});

  //Main auto path
  GenericAuto auto1;
  
  // Create a task that waits until we are 3 inches away from the goal, then deploys
  auto1.add([](){
    task toggleClawTask([](){
      while(odom.pos_diff(odom.get_position(), {.x=32, .y=60}) >= 1); // do nothing
      claw_solenoid.open();
      return 0;
    });
    return true;
  });

  auto1.add([](){return drive.drive_to_point(32, 60, .75, 1, directionType::fwd);});
  auto1.add([](){return drive.drive_to_point(12, 12, .5, 1, directionType::rev);});

  auto1.run(true);
  drive.stop();

  // ========== MAIN LOOP ==========
  while(true)
  {
    
  }

}