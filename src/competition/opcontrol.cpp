#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"
#include "mazegame.h"

// #define AUTO

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========
  // imu.startCalibration(130);
  while (imu.isCalibrating());

  position_t init_pos = {.x=12, .y=4.5, .rot=90};
  odom.set_position(init_pos);

#ifdef AUTO
#define SLOW .6
#define FAST .8



  GenericAuto a;
  a.add([](){ return drive.drive_to_point(10.821915, 42.747610, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(50.156570, FAST); });
  a.add([](){ return drive.drive_to_point(31.967759, 70.716811, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(139.934194, FAST); });
  a.add([](){ return drive.drive_to_point(9.940304, 87.639412, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(81.698977, FAST); });
  a.add([](){ return drive.drive_to_point(9.383733, 123, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(2.302714, FAST); });
  a.add([](){ return drive.drive_to_point(78, 123.725672, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(272.872186, FAST); });
  a.add([](){ return drive.drive_to_point(82.458309, 105, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(187.589328, FAST); });
  a.add([](){ return drive.drive_to_point(58.281359, 99.290299, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(270.546734, FAST); });
  a.add([](){ return drive.drive_to_point(61.918910, 34, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(189.950178, FAST); });
  a.add([](){ return drive.drive_to_point(39.166914, 28.865772, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(276.217439, FAST); });
  a.add([](){ return drive.drive_to_point(41.390902, 9.661834, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(357.948187, FAST); });
  a.add([](){ return drive.drive_to_point(85.979079, 10.379595, FAST, 1, fwd); });
  a.add([](){ return drive.turn_to_heading(89.515782, FAST); });
  a.add([](){ robot_cfg.drive_pid.p = .2; return true; });
  a.add([](){ return drive.drive_to_point(84.360909, 68.5, FAST, 1, fwd); });
  a.run(true);


// #else
  // odom.set_position();
#endif

  MazeGame::init_boundary_lines();
  bool is_end = false;
  timer game_timer;

  // main_controller.ButtonA.pressed([](){
  //   position_t p = odom.get_position();
  //   printf("a.add([](){ return drive.drive_to_point(%f, %f, FAST, 1, fwd); });\na.add([](){ return drive.turn_to_heading(%f, FAST); });\n",
  //           p.x, p.y, p.rot);
  // });

  // ========== LOOP ==========
  while(true)
  {    
    position_t p = odom.get_position();

    if(OdometryBase::pos_diff(p, init_pos) == 0)
      game_timer.reset();
  
    // ========== DRIVING CONTROLS ==========
    drive.drive_arcade(main_controller.Axis3.position() / 100.0, main_controller.Axis1.position() / 100.0, 2);
 
    
    // ========== MANIPULATING CONTROLS ========== 

    MazeGame::is_single_penalty();   
    MazeGame::is_super_mega_ultra_penalty();

    
    if(p.x > 75 && p.y > 68 && p.y < 90 && fabs(imu.roll()) < 5)
    {
      int score = game_timer.time(sec) + (MazeGame::num_penalties * 5) + (MazeGame::num_smups * 45);
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
    // printf("X: %f  Y: %f  rot: %f ", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    // printf("gyro: %f\n", imu.heading(deg));
    // printf("penalties = %d, smups = %d\n", MazeGame::num_penalties, MazeGame::num_smups);
    fflush(stdout);
    fflush(stderr);

    // Wait 20 milliseconds for control loops to calculate time correctly
    vexDelay(20); 
  }

}
