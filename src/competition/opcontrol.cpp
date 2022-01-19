#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"
#include "competition/autonomous.h"

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // ========== INIT ==========
  // imu.calibrate();
  // while(imu.isCalibrating()) {
  //   }
    
  lift_subsys.home();

  // Autonomous::autonomous();
  // odom.set_position({.x=21.7, .y=13.7, .rot=77.5});


  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========
    drive.drive_tank(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);
    // drive.drive_arcade(main_controller.Axis3.position() / 100.0, main_controller.Axis1.position() / 100.0, 2);

    

    // Controls: R1 - increment lift UP | R2 - decrement lift DOWN | X - toggle claw open / closed
    lift_subsys.control(main_controller.ButtonR1.pressing(), main_controller.ButtonR2.pressing(), main_controller.ButtonX.pressing());

    ring_subsys.control(main_controller.ButtonL2.pressing(), main_controller.ButtonL1.pressing());

    // lift_subsys.control(main_controller.ButtonR1.pressing(), main_controller.ButtonL1.pressing(), 
    //                     main_controller.ButtonB.pressing(), main_controller.ButtonA.pressing());
    
    // ring_subsys.control(main_controller.ButtonY.pressing(), main_controller.ButtonR2.pressing()); 

    // if(main_controller.ButtonB.pressing())
    //   odom.set_position(OdometryBase::zero_pos);

    // ========== MANIPULATING CONTROLS ==========

    
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========

    printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    fflush(stdout);
    fflush(stderr);

    // Wait 20 milliseconds for control loops to calculate time correctly
    vexDelay(20); 
  }

}
