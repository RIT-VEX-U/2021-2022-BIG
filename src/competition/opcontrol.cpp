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
  while(imu.isCalibrating()) {vexDelay(100);}

   // while(!drive.drive_forward(24, .5))
  // {
  //   printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
  //   vexDelay(100);
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