#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"
#include "competition/autonomous.h"
#include "subsystems.h"
#include "automation.h"
#include "mazegame.h"

// #define AUTO

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========

  while (imu.isCalibrating());


  odom.set_position({.x=12, .y=4.5, .rot=90});

#ifdef AUTO
#define SLOW .5
#define FAST .9



  GenericAuto a;
  a.run(true);
  a.add([](){ return drive.drive_to_point(12, 55, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(0, FAST); });
  a.add([](){ return drive.drive_to_point(36, 55, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(90, FAST); }); 
  a.add([](){ return drive.drive_to_point(36, 78, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(180, FAST); });
  a.add([](){ return drive.drive_to_point(12, 78, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(90, FAST); });
  a.add([](){ return drive.drive_to_point(12, 123.5, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(0, FAST); });
  a.add([](){ return drive.drive_to_point(82, 123.5, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(270, FAST); });
  a.add([](){ return drive.drive_to_point(82, 101, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(180, FAST); });
  a.add([](){ return drive.drive_to_point(58, 101, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(270, FAST); });
  a.add([](){ return drive.drive_to_point(58, 31, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(180, FAST); });
  a.add([](){ return drive.drive_to_point(36, 31, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(270, FAST); });
  a.add([](){ return drive.drive_to_point(36, 11, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(0, FAST); });
  a.add([](){ return drive.drive_to_point(83, 11, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(90, FAST); });
  a.add([](){ robot_cfg.drive_pid.p=.15; return true; });
  a.add([](){ return drive.drive_to_point(83, 70, SLOW, 1, fwd); });

  // a.run(true);


// #else
  // odom.set_position();
#endif

  MazeGame::init_boundary_lines();
  bool is_end = false;
  timer game_timer;

  // ========== LOOP ==========
  while(true)
  {    
    // ========== DRIVING CONTROLS ==========
    drive.drive_arcade(main_controller.Axis3.position() / 100.0, main_controller.Axis1.position() / 100.0, 2);
 
    
    // ========== MANIPULATING CONTROLS ========== 

    MazeGame::is_single_penalty();   
    MazeGame::is_super_mega_ultra_penalty();

    position_t p = odom.get_position();
    if(fabs(OdometryBase::pos_diff(p, {.x=90, .y=72})) < 5)
    {
      int score = game_timer.time(sec) + (MazeGame::num_penalties * 5) + (MazeGame::num_smups * 20);
      main_controller.Screen.clearScreen();
      main_controller.Screen.setCursor(1, 0);
      main_controller.Screen.print("Penalties: %d", MazeGame::num_penalties + MazeGame::num_smups);
      main_controller.Screen.setCursor(2, 0);
      main_controller.Screen.print("Final Time: %d", score);
      drive.stop();
      return;
    }

#ifndef AUTO
    // static bool reset = true;

    // if(main_controller.ButtonA.pressing())
    // { 
    //   if(reset && drive.drive_to_point(0, 24, 1, 1))
    //     reset = false;

    // } else
    // {
    //   drive.reset_auto();
    //   odom.set_position();
    //   reset = true;
    // }
#endif
   
    // ========== SECONDARY REMOTE ==========

    // ========== AUTOMATION ==========
    
    // printf("L: %f, R: %f, ", left_enc.position(rotationUnits::raw), right_enc.position(rotationUnits::raw));
    printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    // printf("penalties = %d, smups = %d\n", MazeGame::num_penalties, MazeGame::num_smups);
    fflush(stdout);
    fflush(stderr);

    // Wait 20 milliseconds for control loops to calculate time correctly
    vexDelay(20); 
  }

}
