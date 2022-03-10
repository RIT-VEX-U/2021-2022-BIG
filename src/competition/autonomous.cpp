#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"
#include "subsystems.h"
#include "automation.h"

/**
 * Simple autonomous program to rush the goal before the other alliance does
 */
void auto_rush_goal()
{
  
  // task homing_task([](){
  //   lift_subsys.home();
  //   ring_subsys.home();
  //   lift_subsys.set_lift_height(Lift::LiftPosition::DOWN);

  //   while(true)
  //   {
  //     printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
  //     fflush(stdout);
  //     vexDelay(50);
  //   }
  //   return 0;
  // });
  // Odometry set up
  odom.set_position({.x=21.7, .y=13.7, .rot=77.5});

  //Main auto path
  GenericAuto auto1;
  
  // Create a task that waits until we are x inches away from the goal, then deploys
  auto1.add([](){
    task toggleClawTask([](){
      claw_solenoid.open();
      while(odom.pos_diff(odom.get_position(), {.x=32, .y=60}) >= 1); // do nothing
      claw_solenoid.close();
      return 0;
    });
    return true;
  });

  auto1.add([](){return drive.drive_to_point(32, 60, .75, 1, directionType::fwd);});
  auto1.add([](){return drive.drive_to_point(12, 12, .5, 1, directionType::rev);});
  auto1.run(true);
  drive.stop();
  
}

void auto_updated_rush()
{
  // TODO set initial positions and max Y value
  odom.set_position({.x=0, .y=0, .rot=90});
  
  // Set the "end condition" for our initial rush to stop when we:
  // 1. have the goal, or 2. have driven too far in the Y direction
  static bool (*end_condition)(void) = [](){
    return odom.get_position().y > 80 || goal_sense.objectDistance(distanceUnits::in) < 4;
  };

  GenericAuto auto1; 
  // 1 - open the claw and begin the rush towards the goal with the camera
  auto1.add(front_claw::open);
  auto1.add([](){
    return automation::drive_to_goal(.8, end_condition, automation::GoalType::YELLOW);
  });
  // 2 - close the claw and return behind our line
  auto1.add(front_claw::close);
  auto1.add([](){ return drive.drive_to_point(0, 0, .5, 1, directionType::rev); });

  auto1.run(true);
}

/**
 * Simple auto to guarentee an auto win point
 */
void auto_simple_qual()
{
  odom.set_position({.x=22.5, .y=14, .rot=180});

  // lift_subsys.home();

  static bool is_conv_done = false;

  GenericAuto auto1;
  // Start with the fork facing the goal. Drive torwards it, pick it up, deposit preloads
  auto1.add([](){rear_clamp.open(); return true;});
  // auto1.add([](){lift_subsys.set_position(LiftPosition::DOWN); return true;});
  auto1.add([](){return drive.drive_to_point(38, 14, .3, 1, directionType::rev);});
  auto1.add([](){rear_clamp.close(); return true;});
  auto1.add([](){
    task t([](){
      static timer tmr;
      while(tmr.time(sec) < 1.5)
      {
        conveyor_motor.spin(fwd, 12, voltageUnits::volt);
        rear_clamp.close();
      }
      conveyor_motor.stop();
      is_conv_done = true;
      return 0;
      });
    
    return true;
  });

  auto1.add([](){rear_clamp.close(); return drive.drive_to_point(28, 24, .4, 1);});
  auto1.add([](){return is_conv_done;});
  // auto1.add([](){rear_clamp.open(); return true;});
  // auto1.add([](){return drive.drive_to_point(24, 36, .5, 1);});
  // auto1.add([](){return ring_subsys.set_fork_pos(RingCollector::UP);});
  auto1.add([](){claw_solenoid.open(); return true;});
  auto1.add([](){return drive.drive_to_point(36, 63, .4, 1);});
  auto1.add([](){ claw_solenoid.close(); return true;});
  auto1.add([](){return drive.drive_to_point(28, 26, .5, 1, directionType::rev);});

  auto1.run(true);
}

/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  while(imu.isCalibrating());

  // odom.set_position({.x=15.5, .y=11.5, .rot=180});

  // ========== INIT ==========
  
  

  // ========== MAIN LOOP ==========

  // auto_rush_goal();
  auto_simple_qual();
  // skills();

}