#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"
#include "subsystems.h"
#include "automation.h"

enum GoalPosition {LEFT, CENTER};

/**
 * Simple autonomous program to rush the goal before the other alliance does
 */
void auto_rush_goal(GoalPosition pos)
{
  // Determine the robot's initial position based on which goal is selected
  position_t initial_pos;
  if(pos == LEFT)
    initial_pos = {.x=0, .y=0, .rot=0};
  else if (pos == CENTER)
    initial_pos = {.x=0, .y=0, .rot=0};
  odom.set_position(initial_pos);

  // Define the line we cannot cross in auto
  static bool (*return_condition)(void) = [](){
    return odom.get_position().y > 80 || goal_sense.objectDistance(distanceUnits::in) < 3;
    };

  GenericAuto a;
  
  // Initial rush (drive forward w/ camera, close claw, back up)
  a.add(front_claw::open);
  a.add([](){
    return automation::drive_to_goal(.7, return_condition, automation::GoalType::YELLOW);
  });
  a.add(front_claw::close);
  a.add(flaps::lower);

  // Create a "safety task" to make sure we don't cross the line and lose auto
  a.add([](){
    task t([](){
      while(true)
      {
        if(odom.get_position().y > 85)
        {
          front_claw::open();
          return 0;
        } else if(odom.get_position().y < 40)
        {
          return 0;
        }
        
        vexDelay(20);
      }
      return 0;
    });
    return true;
  });

  a.add([](){return drive.drive_to_point(24, 24, .5, 1, directionType::rev);});
  

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

void Autonomous::init_autochooser()
{
  auto_chooser.add("RUSH LEFT");
  auto_chooser.add("RUSH CENTER");
  auto_chooser.add("RUSH L AWP");
  auto_chooser.add("RUSH C AWP");
  auto_chooser.add("AWP");
  auto_chooser.add("AUTO SKILLS");
}

/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  while(imu.isCalibrating());

  // odom.set_position({.x=15.5, .y=11.5, .rot=180});

  // ========== INIT ==========
  
  std::string auto_choice = auto_chooser.get_choice();
  printf("Auto Choice: %s\n", auto_choice.c_str());
  fflush(stdout);
  fflush(stderr);

  if (auto_choice == "AWP")
    auto_simple_qual();
  else if (auto_choice == "RUSH LEFT")
    auto_rush_goal(LEFT);
  else if (auto_choice == "RUSH CENTER")
    auto_rush_goal(CENTER);

  // ========== MAIN LOOP ==========

  // auto_rush_goal();
  // auto_simple_qual();
  // skills();

}