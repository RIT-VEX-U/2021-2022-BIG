#include "competition/opcontrol.h"
#include "robot-config.h"

position_t reset_pos = {0,0,0};

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========
  imu.calibrate();
  while(imu.isCalibrating()) {
    vexDelay(20);
    }


    while(!drive.drive_to_point(24, 24, .5)){vexDelay(20);}
    while(!drive.turn_to_heading(180, .5)){vexDelay(20);}
    while(!drive.drive_to_point(24, 0, .5)){vexDelay(20);}
    while(!drive.turn_to_heading(90, .5)){vexDelay(20);}
    while(!drive.drive_to_point(48, 0, .5)){vexDelay(20);}
    while(!drive.turn_to_heading(-45, .5)){vexDelay(20);}
    while(!drive.drive_to_point(0, 48, .5)){vexDelay(20);}
    while(!drive.turn_to_heading(180, .5)){vexDelay(20);}
    while(!drive.drive_to_point(0, 0, .5)){vexDelay(20);}
    while(!drive.turn_to_heading(0, .5)){vexDelay(20);}


  //  while(!drive.turn_to_heading(90, .5))
  // {
  //   if(main_controller.ButtonA.pressing())
  //     break;

  //   printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
  //   vexDelay(20);
  // }


  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========

    drive.drive_arcade( .5 * main_controller.Axis3.position() / 100.0, .5 * main_controller.Axis1.position() / 100.0, 2);

    if(main_controller.ButtonB.pressing())
      odom.set_position(reset_pos);

    // ========== MANIPULATING CONTROLS ==========

    
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========

    printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    fflush(stdout);
    fflush(stderr);
    // Wait 50 milliseconds for control loops to calculate time correctly
    vexDelay(100); 
  }

}