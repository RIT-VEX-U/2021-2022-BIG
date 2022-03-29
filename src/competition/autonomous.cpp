#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"
#include "subsystems.h"
#include "automation.h"

enum GoalPosition {LEFT, CENTER};

/**
 * Autonomous program to rush the goal before the other alliance does
 */
void auto_rush_goal(GoalPosition pos, bool awp)
{
  // Determine the robot's initial position based on which goal is selected
  position_t initial_pos;
  if(pos == LEFT)
    initial_pos = {.x=0, .y=0, .rot=0};
  else if (pos == CENTER)
    initial_pos = {.x=0, .y=0, .rot=0};
  odom.set_position(initial_pos);

  // Define the line we cannot cross in auto, and distance sensor value
  static bool (*return_condition)(void) = [](){
    return odom.get_position().y > 80 || goal_sense.objectDistance(distanceUnits::in) < 3;
    };

  GenericAuto a;
  
  // Initial rush (drive forward w/ camera, close claw, back up)
  a.add(front_claw::open);
  a.add(flaps::lower);
  a.add([](){ return automation::drive_to_goal(.7, return_condition, automation::GoalType::YELLOW); });
  a.add(front_claw::close);

  // Safety task that will make sure we don't cross the line and lose auto
  a.add_async([](){
    if(odom.get_position().y > 85)
    {
      front_claw::open();
      return true;
    } else if(odom.get_position().y < 40)
    {
      return true;
    }

    return false;
  });

  // Score the goal behind the line
  a.add([](){return drive.drive_to_point(24, 24, .5, 1, directionType::rev);});

  // Stop the auto early if we're not going for the auto win point
  if(!awp)
  {
    a.add([](){ drive.stop(); return true; });
    a.run(true);
    return;
  }

  // Drive backwards in an arc toward the alliance goal, and grab it
  static std::vector<PurePursuit::hermite_point> awp_path1 = {
    {.x=24, .y=24, .dir=deg2rad(90), .mag=50},
    {.x=0, .y=0, .dir=deg2rad(180), .mag=50}
    };

  a.add(rear_claw::open);
  a.add([](){ return drive.pure_pursuit(awp_path1, 12, .4, 20, directionType::rev); });
  a.add(rear_claw::close);

  // Score the rings and move the goal off the platform
  a.add(conveyor::start);
  a.add([](){ return drive.drive_to_point(0, 0, .4, 1, directionType::fwd); });
}

/**
 * Simple auto to guarentee an auto win point
 */
void auto_simple_qual()
{
  odom.set_position({.x=0, .y=0, .rot=180});

  GenericAuto a;

  // Start with the fork facing the goal. Drive torwards it, pick it up, deposit preloads
  a.add(rear_claw::open);
  a.add([](){ return drive.drive_to_point(0, 0, .3, 1, directionType::rev); });
  a.add_delay(100);
  a.add(rear_claw::open);
  a.add(conveyor::start);
  a.add([](){ return drive.drive_to_point(0, 0, .3, 1, directionType::fwd); });
  a.add_delay(3000);
  a.add(conveyor::stop);

  a.run(true);
}

void skills()
{
  static const float FAST = .7;
  static const float NORMAL = .4;
  static const float SLOW = .25;

  odom.set_position({.x=0, .y=0, .rot=90});
  GenericAuto a;

  // Drive backwards and grab the alliance goal
  a.add(rear_claw::open);
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add(rear_claw::close);
  
  // Drive in an arc toward the rings and collect them
  a.add([](){ lift_subsys.set_position(LiftPosition::DRIVING); return true;});
  a.add(conveyor::start);
  
  static std::vector<PurePursuit::hermite_point> path1 = {
    {.x=0, .y=0, .dir=deg2rad(180), .mag=50},
    {.x=0, .y=0, .dir=deg2rad(0), .mag=50},
    {.x=0, .y=0, .dir=deg2rad(0), .mag=50}
  };
  a.add([](){ return drive.pure_pursuit(path1, 12, NORMAL, 20, directionType::fwd); });

  // Stop collecting rings, backup, turn toward the center goal and grab it
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add([](){ return lift_subsys.set_position(DOWN); });
  a.add([](){ return drive.turn_to_heading(90, .5); });
  a.add(conveyor::stop);
  a.add(front_claw::open);
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  a.add(front_claw::close);

  // Take the center goal to the corner of the field
  a.add([](){ return drive.drive_to_point(0, 0, FAST, 1, directionType::fwd); });
  a.add(front_claw::open);

  // Back up
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add([](){ return drive.turn_to_heading(0, NORMAL); });

  // Switch the alliance goal from the rear claw to the front
  a.add(rear_claw::open);
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  a.add([](){ return drive.turn_to_heading(180, NORMAL); });
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::close);
  a.add([](){ return drive.turn_to_heading(0, NORMAL); });

  // Drive to the platform while rasing the lift and place the alliance goal
  a.add([](){ lift_subsys.set_position(UP); return true; });
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  a.add([](){ return drive.turn_to_heading(90, SLOW); });
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::open);

  // Back up, turn and drive backwards towards the other alliance goal and grab it
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::rev); });
  a.add([](){ return drive.turn_to_heading(0, NORMAL); });
  a.add([](){ lift_subsys.set_position(DOWN); return true; });
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add(rear_claw::close);

  // turn toward the line of rings and start collecting them, then back up again
  a.add([](){ lift_subsys.set_position(DRIVING); return true; });
  a.add([](){ return drive.turn_to_heading(270, NORMAL); });
  a.add(conveyor::start);
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add(conveyor::stop);
  a.add([](){ lift_subsys.set_position(DOWN); return true; });

  // Drive towards and grab the other neutral goal with the front claw
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::close);

  // Drive to the platform, and drop off the neutral goal
  a.add([](){ lift_subsys.set_position(UP); return true; });
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  a.add([](){ return drive.turn_to_heading(270, SLOW); });
  a.add(front_claw::open);

  // Back up and switch the alliance goal from back to front
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add([](){ return drive.turn_to_heading(0, NORMAL); });
  a.add([](){ lift_subsys.set_position(DOWN); return true; });
  a.add(rear_claw::open);
  a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  a.add([](){ return drive.turn_to_heading(180, NORMAL); });
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::close);

  // Line up and place second goal on the platform
  a.add([](){ lift_subsys.set_position(UP); return true; });
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::rev); });
  a.add([](){ return drive.turn_to_heading(270, SLOW); });
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::open);

  // Back up to get away from the platform, and finish the auto
  a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::rev); });
  a.add([](){ return lift_subsys.set_position(DOWN); });

  // a.run(true);

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
    auto_rush_goal(LEFT, false);
  else if (auto_choice == "RUSH CENTER")
    auto_rush_goal(CENTER, false);
  else if (auto_choice == "RUSH L AWP")
    auto_rush_goal(LEFT, true);
  else if (auto_choice == "RUSH C AWP")
    auto_rush_goal(CENTER, true);
  else if (auto_choice == "AUTO SKILLS")
    skills();

  // ========== MAIN LOOP ==========

  // auto_rush_goal();
  // auto_simple_qual();
  // skills();

}