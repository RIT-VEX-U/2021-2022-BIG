#include "competition/opcontrol.h"
#include "robot-config.h"

position_t reset_pos = {0,0,0};

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========
  
  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========

    
    // ========== MANIPULATING CONTROLS ==========

    
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========

    // Wait 50 milliseconds for control loops to calculate time correctly
    vexDelay(100); 
  }

}