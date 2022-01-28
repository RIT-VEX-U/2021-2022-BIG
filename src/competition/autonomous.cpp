#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"

void auto_rush_goal()
{
  task homing_task([](){
    lift_subsys.home();
    ring_subsys.home();
    lift_subsys.set_lift_height(Lift::LiftPosition::DOWN);

    while(true)
    {
      printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
      fflush(stdout);
      vexDelay(50);
    }
    return 0;
  });
  // Odometry set up
  odom.set_position({.x=21.7, .y=13.7, .rot=77.5});

  //Main auto path
  GenericAuto auto1;
  
  // Create a task that waits until we are x inches away from the goal, then deploys
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
}

void auto_simple_qual()
{
  odom.set_position({.x=22.5, .y=14, .rot=180});

  ring_subsys.home();
  lift_subsys.home();

  static bool is_conv_done = false;

  GenericAuto auto1;
  // Start with the fork facing the goal. Drive torwards it, pick it up, deposit preloads
  auto1.add([](){return ring_subsys.set_fork_pos(RingCollector::ForkPosition::DOWN);});
  auto1.add([](){lift_subsys.set_lift_height(Lift::DOWN); return true;});
  auto1.add([](){return drive.drive_to_point(35, 12, .4, 1, directionType::rev);});
  auto1.add([](){return ring_subsys.set_fork_pos(RingCollector::ForkPosition::LOADING);});
  auto1.add([](){
    task t([](){
      static timer tmr;
      while(tmr.time(sec) < 1.5)
      {
        conveyor_motor.spin(fwd, 12, voltageUnits::volt);
        ring_subsys.set_fork_pos(RingCollector::LOADING);
      }
      conveyor_motor.stop();
      is_conv_done = true;
      return 0;
      });
    
    return true;
  });

  auto1.add([](){ring_subsys.set_fork_pos(RingCollector::LOADING); return drive.drive_to_point(24, 24, .5, 1);});
  auto1.add([](){return is_conv_done;});
  auto1.add([](){return ring_subsys.set_fork_pos(RingCollector::DOWN);});
  auto1.add([](){return drive.drive_to_point(24, 36, .5, 1);});
  auto1.add([](){return ring_subsys.set_fork_pos(RingCollector::UP);});
  auto1.add([](){claw_solenoid.close(); return true;});
  auto1.add([](){return drive.drive_to_point(30, 59, .4, 1);});
  auto1.add([](){ claw_solenoid.open(); return true;});
  auto1.add([](){return drive.drive_to_point(16, 24, .5, 1, directionType::rev);});

  auto1.run(true);
}

void skills()
{
  odom.set_position({.x=15.5, .y=11.5, .rot=180});

  ring_subsys.home();
  lift_subsys.home();

  // General "clean out the conveyor" task
  state_ptr finish_conveying = [](){
    task t([](){
      static timer tmr;
      tmr.reset();
      while(tmr.time(sec) < 2)
      {
        conveyor_motor.spin(fwd, 12, volt);
        vexDelay(20);
      }
      conveyor_motor.stop();
      return 0;
    });
    
    return true;    
  };

  // General "open claw" task with delay
  static state_ptr open_claw = [](){
    static timer t;
    t.reset();
    claw_solenoid.close();
    while(t.time(sec) < .5);
    return true;
  };

  // General "close claw" task with delay
  static state_ptr close_claw = [](){
    static timer t;
    t.reset();
    claw_solenoid.open();
    while(t.time(sec) < .5);
    return true;
  };

  GenericAuto a;

  // Start with the fork facing the goal on the ramp; drive and pick it up
  a.add([](){return ring_subsys.set_fork_pos(RingCollector::DOWN);});
  a.add([](){return lift_subsys.set_lift_height(Lift::DOWN);});
  a.add([](){return drive.drive_to_point(30, 11.5, .4, 1, directionType::rev);});
  a.add([](){return ring_subsys.set_fork_pos(RingCollector::LOADING);});
  a.add(finish_conveying);

  //path to 1st rings
  static std::vector<PurePursuit::hermite_point> p1 = {{.x=30, .y=11.5, .dir=deg2rad(180), .mag=75}, {.x=28, .y=47, .dir=deg2rad(0), .mag=50}};
  a.add([](){return drive.pure_pursuit(p1, 6, .3, 50);});
  a.add([](){
    lift_subsys.set_lift_height(Lift::DRIVING);
    return true;
  });
  a.add([](){return drive.turn_to_heading(0, 1);});

  // Pick up 1st line of rings
  a.add([](){
    conveyor_motor.spin(fwd, 12, volt);
    return drive.drive_to_point(82, 47, .3, 1);
    });
  a.add(finish_conveying);

  //Back up and turn to the centre goal
  a.add([](){return drive.drive_to_point(70, 47, .3, 1, directionType::rev);});
  a.add([](){lift_subsys.set_lift_height(Lift::DOWN); claw_solenoid.close(); return drive.turn_to_heading(90, 1);});

  // Grab theh goal, and drive it to the corner of the field
  a.add([](){return drive.drive_to_point(70, 60, .3, 1);});
  a.add([](){claw_solenoid.open(); return true;});
  a.add([](){return drive.drive_to_point(23, 122, .5, 1);});
  a.add([](){claw_solenoid.close(); return true;});

  // Back up, turn around and drive across the field to the opposite alliace goal
  a.add([](){return drive.drive_to_point(29, 110, .4, 1, directionType::rev);});
  a.add([](){return drive.turn_to_heading(0, 1);});
  a.add([](){return drive.drive_to_point(115, 110, .6, 1);});
  // X1: 29, Y1: 110, X2=121, Y2=131

  // Swing around and grab the goal
  static std::vector<PurePursuit::hermite_point> p2 = {
    {.x=115, .y=110, .dir=deg2rad(0), .mag=30},
    {.x=115, .y=134, .dir=deg2rad(180), .mag=50}};
  a.add([](){return drive.pure_pursuit(p2, 6, .3, 50);});
  a.add([](){return drive.turn_to_heading(180, 1);});
  a.add([](){return drive.drive_to_point(104, 134, .3, 1);});
  a.add([](){claw_solenoid.open(); return true;});

  // basically a 3 point turn to get out of the corner
  a.add([](){return drive.drive_to_point(123, 125, .3, 1, directionType::rev);});
  a.add([](){return drive.drive_to_point(96.5, 109, .3, 1);});
  a.add([](){return drive.turn_to_heading(180, 1);});

  // Begin switching goals
  a.add([](){return drive.drive_to_point(65, 109, .4, 1);});

  // -> Set fork down and drive forwards
  a.add([](){return ring_subsys.set_fork_pos(RingCollector::DOWN);});
  a.add([](){return drive.drive_to_point(44, 109, .3, 1);});
  a.add([](){return drive.turn_to_heading(180, .7);});

  // -> open claw, backup and turn around
  a.add([](){
    ring_subsys.set_fork_pos(RingCollector::UP);
    return open_claw();
  });
  a.add([](){return drive.drive_to_point(48, 109, .4, 1, directionType::rev);});
  a.add([](){return drive.turn_to_heading(0, .7);});
  a.add([](){return ring_subsys.set_fork_pos(RingCollector::DOWN);});

  // -> grab the goal with the claw, back up, grab the goal with the fork
  a.add([](){return drive.drive_to_point(63, 109.5, .3, 1);});
  a.add(close_claw);
  a.add([](){return drive.drive_to_point(38, 109, .3, 1, directionType::rev);});
  a.add([](){return ring_subsys.set_fork_pos(RingCollector::DRIVING);});
  a.add([](){lift_subsys.set_lift_height(Lift::PLATFORM); return true;});

  // Drive to goal and place down
  a.add([](){return drive.drive_to_point(64, 106, .3, 1);});
  a.add([](){return drive.turn_to_heading(90, .5);});
  a.add(open_claw);
  
  // reverse, lower lift, and prepare to get next alliance goal
  a.add([](){return drive.drive_to_point(64, 102, .3, 1, directionType::rev);});
  a.add([](){return drive.turn_to_heading(180, .6);});
  a.add([](){lift_subsys.set_lift_height(Lift::DRIVING); return true;});

  // Drive to the goal fast, then slow, then grab it
  a.add([](){conveyor_motor.spin(fwd, 12, volt);return drive.drive_to_point(48, 106, .4, 1);});
  a.add([](){lift_subsys.set_lift_height(Lift::DOWN); conveyor_motor.stop(); return drive.drive_to_point(14, 106, .3, 1);});
  a.add(close_claw);

  // Lift the goal off the ground, spin towards rings, and begin collecting
  a.add([](){lift_subsys.set_lift_height(Lift::DRIVING); ring_subsys.set_fork_pos(RingCollector::LOADING); return true;});
  a.add([](){return drive.drive_to_point(25, 105, .3, 1, directionType::rev);});
  a.add([](){lift_subsys.set_lift_height(Lift::UP); return drive.turn_to_heading(270, 1);});
  a.add([](){return drive.drive_to_point(23, 23, .4, 1);});

  // a.add([](){conveyor_motor.spin(fwd, 12, volt); return drive.drive_to_point(14, 55, .3, 1);});
  // a.add(finish_conveying);
  // a.add([](){return drive.turn_to_heading(340, .6);});


  a.run(true);

  while(true)
  {
    ring_subsys.hold_thread = false;
    lift_subsys.hold = false;
    fork_motor.stop();
    lift_motors.stop();
    printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    fflush(stdout);
    vexDelay(20);
  }
  printf("Auto finished\n");
}
/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  while(imu.isCalibrating());

  odom.set_position({.x=15.5, .y=11.5, .rot=180});

  // ========== INIT ==========
  
  

  // ========== MAIN LOOP ==========

  // auto_rush_goal();
  // auto_simple_qual();
  skills();

}