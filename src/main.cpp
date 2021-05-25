/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       RIT VEXU                                                  */
/*    Created:      Mon May 24 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "competition/opcontrol.h"
#include "competition/autonomous.h"

using namespace vex;

competition comp;

/**
 * The main method of the robot.
 * Do NOT use for general programming or initialization.
 * Instead use competition/opcontrol.cpp, competition/autonomous.cpp, or robot-config.cpp for programming or configuration.
 */
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Set the driver control and autonomous periods to point
  // to the correct methods
  comp.autonomous(Autonomous::autonomous);
  comp.drivercontrol(OpControl::opcontrol);

  while(true)
  {
    //Do Nothing
    vexDelay(100);
  }
  
}
