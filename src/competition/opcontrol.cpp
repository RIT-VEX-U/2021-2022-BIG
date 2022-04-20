#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"
#include "competition/autonomous.h"
#include "subsystems.h"
#include "automation.h"

void tuning(bool btn)
{
  static bool reset = true;
  if(btn)
  {
    if(reset && drive.drive_to_point(0, 24, .7, 1, fwd))
      reset = false;    
  }else
  {
    drive.reset_auto();
    odom.set_position();
    reset = true;
  }
}

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========
  // imu.calibrate();
  // while(imu.isCalibrating());

  // Autonomous::autonomous();

  // ========== LOOP ==========
  while(true)
  {    
    // bool is_auto_aiming = main_controller.ButtonL2.pressing() && main_controller.ButtonR2.pressing();

    // ========== DRIVING CONTROLS ==========
    // if(!is_auto_aiming)
    // if (!main_controller.ButtonUp.pressing())
    drive.drive_tank(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);

    // if(main_controller.ButtonUp.pressing())
    //   automation::drive_to_goal(1, [](){ return dist.objectDistance(distanceUnits::mm) < 35;}, automation::YELLOW);
    // tuning(main_controller.ButtonUp.pressing());
    // drive.drive_arcade(main_controller.Axis3.position() / 100.0, main_controller.Axis1.position() / 100.0, 2);
    

    // ========== MANIPULATING CONTROLS ==========

    // Controls: R1 - increment lift UP | R2 - decrement lift DOWN | X - toggle claw open / closed
    // if(!is_auto_aiming)
    // {
    //   if(!conveyor::is_running)
    // if(main_controller.ButtonA.pressing())
    //   lift_subsys.set_position(PLATFORM);
    // else
    lift_subsys.control_manual(main_controller.ButtonR1.pressing(), main_controller.ButtonR2.pressing(), 12, 12);
    // }

    // if(!is_auto_aiming)
    rear_claw::control(main_controller.ButtonL2.pressing());
      
    front_claw::control(main_controller.ButtonX.pressing());
    conveyor::control(main_controller.ButtonL1.pressing());
    
   
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========

    // if(is_auto_aiming)
    //   automation::drive_with_autoaim(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);

    

    // printf("lift pos: %f\n", l_lift.position(rotationUnits::rev));
    printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    fflush(stdout);
    fflush(stderr);

    // Wait 20 milliseconds for control loops to calculate time correctly
    vexDelay(20); 
  }

}
