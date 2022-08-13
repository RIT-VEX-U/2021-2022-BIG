#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"
#include "subsystems.h"
#include "automation.h"

enum GoalPosition {LEFT, CENTER};

/**
 * Autonomous program to rush the goal before the other alliance does
 */
void auto_rush_goal(GoalPosition pos, bool awp_l, bool awp_r)
{
  odom.set_position({.x=23.5, .y=11.25, .rot=82.5});

  // Define the line we cannot cross in auto, and distance sensor value
  static bool (*return_condition)(void) = [](){
    double goal_dist = dist.objectDistance(mm);
    return odom.get_position().y > 72 
    || (goal_dist > 0 && goal_dist < 50);
    };

  GenericAuto a;
  
  static timer auto_timer;
  auto_timer.reset();

  // INIT LIFT STOP!!!
  a.add([](){
    lift_subsys.set_async(false); 
    lift_motors.stop();
    return true; });

  // If going after the center goal, drive forward and cut to the right before rushing
  if(pos == CENTER)
  {
    // a.add([](){return drive.drive_to_point(28, 51, .5, 1, fwd);});
    // a.add([](){ return drive.turn_to_heading(33, .5); });
  }
  
  // Initial rush (drive forward w/ camera, close claw, back up)
  a.add(front_claw::open);
  a.add([](){ return automation::drive_to_goal(.7, return_condition, automation::GoalType::YELLOW); });
  a.add(front_claw::close);

  // Drive back the way we came for the center rush before backing up
  if(pos == CENTER)
  {
    a.add_delay(100);
    a.add([](){ return lift_subsys.set_position(LOW);});
    // a.add([](){ return drive.drive_to_point(28, 51, .5, 1, directionType::rev);});
  }

  // Score the goal behind the line, further back if we're not going after the AWP
  // if(awp_l == true)
    // a.add([](){return drive.drive_to_point(22, 33, .5, 1, directionType::rev);});
  // else
    // a.add([](){ return drive.drive_to_point(22, 16, .5, 1, directionType::rev);});


  // Stop the auto early if we're not going for the auto win point
  if(!awp_l)
  {
    a.add([](){ drive.stop(); return true; });
    a.run(true);
    return;
  }

  // Raise the lift a bit for drag reduction
  a.add([](){ lift_subsys.set_position(LOW); return true;});
  a.add(rear_claw::open);

  static timer all_goal_tmr;

  // 2 stage turning, it's bit tight back there
  // a.add([](){ return drive.drive_to_point(22, 12, .3, 1, directionType::rev);});
  // a.add([](){ all_goal_tmr.reset(); return drive.turn_to_heading(175, .5);});
  // a.add([](){ return drive.drive_to_point(35, 11.5, .3, 1, directionType::rev) || all_goal_tmr.time(sec) > 3; });

  // Grab the goal, begin the conveyor and start driving away
  a.add(rear_claw::close);
  a.add_delay(100);
  a.add_async([](){ conveyor::start(); vexDelay(1000); conveyor::stop(); return true;});
  // a.add([](){ return drive.drive_to_point(24, 18.5, .3, 1, directionType::fwd);});

  // Turn toward the field in preperation for driver control
  // a.add([](){ return drive.turn_to_heading(45, .5); });
  
  // If we're stopping at the left-side goal
  if (!awp_r)
  {
    a.add([](){ drive.stop(); return true; });
    a.run(true);
    return;
  }

  // Drop off the goal
  a.add(rear_claw::open);
  // a.add([](){ return drive.drive_to_point(30, 31, .3, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(180, .5); });

  // Drive across the field
  // a.add([](){ return drive.drive_to_point(100, 30, .5, 1, directionType::rev); });
  a.add([](){ return automation::drive_to_goal(.35, [](){ return rear_switch.pressing() == 1;}, automation::ANY, directionType::rev); });
  // a.add([](){ return drive.drive_to_point(122, 36, .2, 1, directionType::rev); });
  a.add([](){ drive.drive_tank(-.15, -.15); vexDelay(1000); drive.stop(); return true;});
  a.add_delay(300);
  a.add(rear_claw::close);
  a.add_delay(100);

  // Turn toward the rings
  a.add([](){ lift_subsys.set_position(UP); return true;}); 
  // a.add([](){ return drive.drive_to_point(118, 36, .3, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(270, .5); });

  // Match timer, make sure we drop the goal at the end
  a.add_async([](){
    while(auto_timer.time(sec) < 44) {vexDelay(200);}
    rear_claw::open();
    return true;
  });

  // Collect
  static timer col_tmr;
  a.add(conveyor::start);
  a.add_delay(1000);
  // a.add([](){ return drive.drive_to_point(114, 11, .3, 1, directionType::fwd) || col_tmr.time(sec) > 3; });
  a.add([](){ drive.reset_auto(); return true; });
  // a.add([](){ col_tmr.reset(); return drive.drive_to_point(115, 36, .4, 1, directionType::rev); });
  a.add_delay(1000);
  // a.add([](){ return drive.drive_to_point(115, 11, .3, 1, directionType::fwd) || col_tmr.time(sec) > 3; });
  a.add([](){ drive.reset_auto(); return true; });
  // a.add([](){ col_tmr.reset(); return drive.drive_to_point(118, 36, .4, 1, directionType::rev); });
  a.add_delay(1000);
  // a.add([](){ return drive.drive_to_point(118, 11, .3, 1, directionType::fwd) || col_tmr.time(sec) > 3; });
  a.add([](){ drive.reset_auto(); return true; });
  // a.add([](){ col_tmr.reset(); return drive.drive_to_point(118, 36, .4, 1, directionType::rev); });
  a.add_delay(1000);
  // a.add([](){ return drive.drive_to_point(118, 11, .3, 1, directionType::fwd) || col_tmr.time(sec) > 3; });
  a.add([](){ drive.reset_auto(); return true; });
  // a.add([](){ col_tmr.reset(); return drive.drive_to_point(118, 24, .4, 1, directionType::rev); });

  // Turn around & drop

  a.add([](){ lift_subsys.set_position(DOWN); return true;});
  a.add(rear_claw::open);
  a.add(conveyor::stop);

  // Return behind line
  // a.add_async([](){ vexDelay(1000); conveyor::stop(); return true; });
  // a.add([](){ return drive.drive_to_point(118, 19, .5, 1, directionType::rev); });

  a.add([](){ drive.stop(); return true; });
  a.run(true);
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
  // a.add([](){ return drive.drive_to_point(0, 0, .3, 1, directionType::rev); });
  a.add_delay(100);
  a.add(rear_claw::open);
  a.add(conveyor::start);
  // a.add([](){ return drive.drive_to_point(0, 0, .3, 1, directionType::fwd); });
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
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add(rear_claw::close);
  
  // Drive in an arc toward the rings and collect them
  a.add([](){ lift_subsys.set_position(LiftPosition::DRIVING); return true;});
  a.add(conveyor::start);
  
  static std::vector<PurePursuit::hermite_point> path1 = {
    {.x=0, .y=0, .dir=deg2rad(180), .mag=50},
    {.x=0, .y=0, .dir=deg2rad(0), .mag=50},
    {.x=0, .y=0, .dir=deg2rad(0), .mag=50}
  };
  // a.add([](){ return drive.pure_pursuit(path1, 12, NORMAL, 20, directionType::fwd); });

  // Stop collecting rings, backup, turn toward the center goal and grab it
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add([](){ return lift_subsys.set_position(DOWN); });
  // a.add([](){ return drive.turn_to_heading(90, .5); });
  a.add(conveyor::stop);
  a.add(front_claw::open);
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  a.add(front_claw::close);

  // Take the center goal to the corner of the field
  // a.add([](){ return drive.drive_to_point(0, 0, FAST, 1, directionType::fwd); });
  a.add(front_claw::open);

  // Back up
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  // a.add([](){ return drive.turn_to_heading(0, NORMAL); });

  // Switch the alliance goal from the rear claw to the front
  a.add(rear_claw::open);
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(180, NORMAL); });
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::close);
  // a.add([](){ return drive.turn_to_heading(0, NORMAL); });

  // Drive to the platform while rasing the lift and place the alliance goal
  a.add([](){ lift_subsys.set_position(UP); return true; });
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(90, SLOW); });
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::open);

  // Back up, turn and drive backwards towards the other alliance goal and grab it
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::rev); });
  // a.add([](){ return drive.turn_to_heading(0, NORMAL); });
  a.add([](){ lift_subsys.set_position(DOWN); return true; });
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add(rear_claw::close);

  // turn toward the line of rings and start collecting them, then back up again
  a.add([](){ lift_subsys.set_position(DRIVING); return true; });
  // a.add([](){ return drive.turn_to_heading(270, NORMAL); });
  a.add(conveyor::start);
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::rev); });
  a.add(conveyor::stop);
  a.add([](){ lift_subsys.set_position(DOWN); return true; });

  // Drive towards and grab the other neutral goal with the front claw
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::close);

  // Drive to the platform, and drop off the neutral goal
  a.add([](){ lift_subsys.set_position(UP); return true; });
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(270, SLOW); });
  a.add(front_claw::open);

  // Back up and switch the alliance goal from back to front
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(0, NORMAL); });
  a.add([](){ lift_subsys.set_position(DOWN); return true; });
  a.add(rear_claw::open);
  // a.add([](){ return drive.drive_to_point(0, 0, NORMAL, 1, directionType::fwd); });
  // a.add([](){ return drive.turn_to_heading(180, NORMAL); });
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::close);

  // Line up and place second goal on the platform
  a.add([](){ lift_subsys.set_position(UP); return true; });
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::rev); });
  // a.add([](){ return drive.turn_to_heading(270, SLOW); });
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::fwd); });
  a.add(front_claw::open);

  // Back up to get away from the platform, and finish the auto
  // a.add([](){ return drive.drive_to_point(0, 0, SLOW, 1, directionType::rev); });
  a.add([](){ return lift_subsys.set_position(DOWN); });

  // a.run(true);

}

void Autonomous::init_autochooser()
{
  auto_chooser.add("RUSH LEFT");
  auto_chooser.add("RUSH CENTER");
  auto_chooser.add("RUSH L AWP");
  auto_chooser.add("RUSH C AWP");
  auto_chooser.add("FULL LEFT");
  auto_chooser.add("FULL CENTER");
  // auto_chooser.add("AUTO SKILLS");
}

/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  lift_subsys.set_async(false);
  while(imu.isCalibrating());

  //testing
  // auto_rush_goal(LEFT, true, true);
  // return;

  // odom.set_position({.x=15.5, .y=11.5, .rot=180});

  // ========== INIT ==========
  
  std::string auto_choice = auto_chooser.get_choice();
  printf("Auto Choice: %s\n", auto_choice.c_str());
  fflush(stdout);
  fflush(stderr);

  if (auto_choice == "RUSH LEFT")
    auto_rush_goal(LEFT, false, false);
  else if (auto_choice == "RUSH CENTER")
    auto_rush_goal(CENTER, false, false);
  else if (auto_choice == "RUSH L AWP")
    auto_rush_goal(LEFT, true, false);
  else if (auto_choice == "RUSH C AWP")
    auto_rush_goal(CENTER, true, false);
  else if (auto_choice == "FULL LEFT")
    auto_rush_goal(LEFT, true, true);
  else if (auto_choice == "FULL CENTER")
    auto_rush_goal(CENTER, true, true);

  // ========== MAIN LOOP ==========
  // Nothing here, dummy!

}