#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"
#include "competition/autonomous.h"
#include "subsystems.h"
#include "automation.h"

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========
  // imu.calibrate();
  // while(imu.isCalibrating());
  
  // flaps.close();

  // while(true)
  // {
  //   position_t pos = odom.get_position();
  //   printf("X: %f, Y: %f, ROT: %f\n", pos.x, pos.y, pos.rot);
  //   vexDelay(20);
  // }

  // return;

  // ========== LOOP ==========
  while(true)
  {    
    bool is_auto_aiming = main_controller.ButtonL2.pressing() && main_controller.ButtonR2.pressing();

    // ========== DRIVING CONTROLS ==========
    if(!is_auto_aiming)
      drive.drive_tank(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);
    // drive.drive_arcade(main_controller.Axis3.position() / 100.0, main_controller.Axis1.position() / 100.0, 2);
    

    // ========== MANIPULATING CONTROLS ==========

    // Controls: R1 - increment lift UP | R2 - decrement lift DOWN | X - toggle claw open / closed
    if(!is_auto_aiming)
    {
      if(!conveyor::is_running)
        lift_subsys.control_manual(main_controller.ButtonR1.pressing(), main_controller.ButtonR2.pressing(), 12, 4);
      else
        lift_subsys.set_position(LiftPosition::PLATFORM);
    }

    if(!is_auto_aiming)
      rear_claw::control(main_controller.ButtonL2.pressing());
      
    front_claw::control(main_controller.ButtonX.pressing());
    conveyor::control(main_controller.ButtonL1.pressing());
    
   
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========

    if(is_auto_aiming)
      automation::drive_with_autoaim(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);

    // printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    fflush(stdout);
    fflush(stderr);

    // Wait 20 milliseconds for control loops to calculate time correctly
    vexDelay(20); 
  }

}
