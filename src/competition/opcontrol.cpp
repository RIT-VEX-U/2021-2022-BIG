#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"

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
  // odom.set_position({.x = 24, .y=14, .rot=90});
  // odom.set_position();
  lift_subsys.home();
  // odom.set_position({.x = 24, .y=14, .rot=90}); // starting pos

  // static std::vector<PurePursuit::hermite_point> path_to_neutral = {{.x = 24, .y=14, .dir=deg2rad(90), .mag=25},{.x=36, .y=55, .dir=deg2rad(90), .mag=25}};    
  // GenericAuto a1;
  // a1.add([](){return drive.pure_pursuit(path_to_neutral, 6, .5, 20);});
  // a1.add([](){return drive.drive_to_point(36, 62, .2, 1);});
  // a1.add([](){claw_solenoid.open(); return true;});
  // a1.add([](){return drive.drive_to_point(24, 12, .5, 1, directionType::rev);});

  // a1.run(true);
    

  // std::vector<Vector::point_t> path = {{0,0}, {10,4}, {20,10},{25,25}};

  

  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========
    drive.drive_tank(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);

    

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
