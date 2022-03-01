#include "competition/opcontrol.h"
#include "robot-config.h"
#include "../core/include/utils/generic_auto.h"
#include "competition/autonomous.h"

/**
 * Contains the main loop of the robot code while running in the driver-control period.
 */
void OpControl::opcontrol() 
{
  // Autonomous::autonomous();
  // ========== INIT ==========
  // imu.calibrate();
  // while(imu.isCalibrating());
    
  // lift_subsys.home();
  // ring_subsys.home();

  // flaps.close();

  // printf("Down: %f, Driving: %f, Platform: %f, Up: %f\n", lift_map[DOWN], lift_map[DRIVING], lift_map[PLATFORM], lift_map[UP]);
  // lift_subsys.set_position(LiftPosition::DOWN);
  // Autonomous::autonomous();
  // odom.set_position({.x=22.5, .y=14, .rot=180});

  // while(true)
  // {
  //   position_t pos = odom.get_position();
  //   printf("X: %f, Y: %f, ROT: %f\n", pos.x, pos.y, pos.rot);
  //   vexDelay(20);
  // }

  // return;

  // main_controller.ButtonY.pressed([](){flaps.set(!flaps.value());});

  // ========== LOOP ==========
  while(true)
  {
    // ========== DRIVING CONTROLS ==========
    drive.drive_tank(main_controller.Axis3.position() / 100.0, main_controller.Axis2.position() / 100.0, 2);
    // drive.drive_arcade(main_controller.Axis3.position() / 100.0, main_controller.Axis1.position() / 100.0, 2);

    
    // ========== MANIPULATING CONTROLS ==========

    // Controls: R1 - increment lift UP | R2 - decrement lift DOWN | X - toggle claw open / closed
    // lift_subsys.control(main_controller.ButtonR1.pressing(), main_controller.ButtonR2.pressing(), main_controller.ButtonX.pressing());
    lift_subsys.control_continuous(main_controller.ButtonR1.pressing(), main_controller.ButtonR2.pressing());


    // Control the clamp
    static bool clamp_last_pressed = false;
    if(!clamp_last_pressed && main_controller.ButtonX.pressing())
      claw_solenoid.set( !(claw_solenoid.value() == 1) );
    clamp_last_pressed = main_controller.ButtonX.pressing();

    // Control the backside clamp
    static bool back_clamp_last = false;
    if(!back_clamp_last && main_controller.ButtonL2.pressing())
      rear_clamp.set( !(rear_clamp.value() == 1) );
    back_clamp_last = main_controller.ButtonL2.pressing();

    // Control the ring subsystem
    static bool is_collecting_rings = false;
    static bool last_ring_btn = false;
    static timer overcurrent_tmr;

    if(!last_ring_btn && main_controller.ButtonL1.pressing())
      is_collecting_rings = !is_collecting_rings;
    last_ring_btn = main_controller.ButtonL1.pressing();

    if(main_controller.ButtonB.pressing())
    {
      conveyor_motor.spin(directionType::rev, 12, volt);
    } else if(is_collecting_rings)
    {
      // if(conveyor_motor.current(currentUnits::amp) > 2.0)
      //   overcurrent_tmr.reset();

      printf("%f\n", conveyor_motor.current(currentUnits::amp));

      // if(overcurrent_tmr.time(sec) < 2)
      //   conveyor_motor.spin(directionType::rev, 12, volt);
      // else
      conveyor_motor.spin(fwd, 12, volt);
      
    }
    else
    {
      conveyor_motor.stop();
    }

    // ring_subsys.control(main_controller.ButtonL2.pressing(), main_controller.ButtonL1.pressing());
    
   
    // ========== SECONDARY REMOTE ==========


    // ========== AUTOMATION ==========


    // printf("X: %f  Y: %f  rot: %f\n", odom.get_position().x,odom.get_position().y, odom.get_position().rot);
    // printf("lift pos: %f\n", lift_motors.position(rotationUnits::rev));
    fflush(stdout);
    fflush(stderr);

    // Wait 20 milliseconds for control loops to calculate time correctly
    vexDelay(20); 
  }

}
