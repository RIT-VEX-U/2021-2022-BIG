#include "competition/opcontrol.h"
#include "robot-config.h"

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========

  while(!drive.turn_degrees(90, .5))
  {
    printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    vexDelay(100);
  }

  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========

    drive.drive_tank(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0);

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