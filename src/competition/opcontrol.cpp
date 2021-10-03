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


    // while(!drive.drive_to_point(24, 24, .5, .25)){vexDelay(20);}
    // while(!drive.turn_to_heading(270, .5)){vexDelay(20);}
    // while(!drive.drive_to_point(24, 0, .5, .25)){vexDelay(20);}
    // while(!drive.turn_to_heading(180, .5)){vexDelay(20);}
    // while(!drive.drive_to_point(48, 0, .5, .25)){vexDelay(20);}
    // while(!drive.turn_to_heading(-45, .5)){vexDelay(20);}
    // while(!drive.drive_to_point(0, 48, .5, .25)){vexDelay(20);}
    // while(!drive.turn_to_heading(270, .5)){vexDelay(20);}
    // while(!drive.drive_to_point(0, 0, .5, .25)){vexDelay(20);}
    // while(!drive.turn_to_heading(90, .5)){vexDelay(20);}

    GenericAuto a1;
    a1.add([](){return drive.turn_to_heading(45, .5);}); 
    a1.add([](){return drive.drive_to_point(24, 24, .5, .5);});
    a1.add([](){return drive.turn_to_heading(270, .5);});
    a1.add([](){return drive.drive_to_point(24, 0, .5, .5);});
    a1.add([](){return drive.turn_to_heading(0, .5);});
    a1.add([](){return drive.drive_to_point(48, 0, .5, .5);});
    a1.add([](){return drive.turn_to_heading(90, .5);});
    a1.add([](){return drive.drive_to_point(48, 48, .5, .5);});
    a1.add([](){return drive.turn_to_heading(225, .5);});
    a1.add([](){return drive.drive_to_point(0, 0, .5, .5);});
    a1.add([](){return drive.turn_to_heading(90, .5);});

    // a1.run(true);



  //  while(!drive.drive_forward(24, .5))
  // {
  //   if(main_controller.ButtonA.pressing())
  //     break;

  //   printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
  //   vexDelay(20);
  // }

  position_t test_pos = {.x=24, .y=24};


  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========

    drive.drive_arcade( .5 * main_controller.Axis3.position() / 100.0, .5 * main_controller.Axis1.position() / 100.0, 2);

    if(main_controller.ButtonB.pressing())
      odom.set_position(OdometryBase::zero_pos);

    


    // ========== MANIPULATING CONTROLS ==========

    
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========

    double dist_norm = OdometryBase::pos_diff(odom.get_position(), test_pos, true);
    double dist_axis = OdometryBase::pos_diff(odom.get_position(), test_pos, true, true);

    printf("dist (axis): %f, dist (norm): %f, X: %f  Y: %f  rot: %f\n",dist_axis, dist_norm, odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    fflush(stdout);
    fflush(stderr);
    // Wait 50 milliseconds for control loops to calculate time correctly
    vexDelay(100); 
  }

}