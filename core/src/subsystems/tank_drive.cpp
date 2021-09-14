#include "../core/include/subsystems/tank_drive.h"

TankDrive::TankDrive(motor_group &left_motors, motor_group &right_motors, inertial &gyro_sensor, TankDrive::tankdrive_config_t &config, OdometryTank *odom)
    : left_motors(left_motors), right_motors(right_motors),
     drive_pid(config.drive_pid), turn_pid(config.turn_pid), odometry(odom)
{

}

/**
 * Stops rotation of all the motors using their "brake mode"
 */
void TankDrive::stop()
{
  left_motors.stop();
  right_motors.stop();
}

/**
 * Drive the robot using differential style controls. left_motors controls the left motors,
 * right_motors controls the right motors.
 * 
 * left_motors and right_motors are in "percent": -1.0 -> 1.0
 */
void TankDrive::drive_tank(double left, double right)
{
  left_motors.spin(directionType::fwd, left * 100, velocityUnits::pct);
  right_motors.spin(directionType::fwd, right * 100, velocityUnits::pct);
}

/**
 * Drive the robot using arcade style controls. forward_back controls the linear motion,
 * left_right controls the turning.
 * 
 * left_motors and right_motors are in "percent": -1.0 -> 1.0
 */
void TankDrive::drive_arcade(double forward_back, double left_right)
{
  double left = forward_back + left_right;
  double right = forward_back - left_right;

  left_motors.setVelocity(left * 100, velocityUnits::pct);
  right_motors.setVelocity(right * 100, velocityUnits::pct);
}

/**
 * Autonomously drive the robot X inches forward (Negative for backwards), with a maximum speed
 * of percent_speed (-1.0 -> 1.0).
 * 
 * Uses a PID loop for it's control.
 * 
 * NOTE: uses relative positioning, so after a few drive_forward's, position may be lost!
 */
bool TankDrive::drive_forward(double inches, double percent_speed)
{
  // On the first run of the funciton, reset the motor position and PID
  if (initialize_func)
  {
    saved_pos = odometry->get_position();
    drive_pid.reset();

    drive_pid.set_limits(-fabs(percent_speed), fabs(percent_speed));
    drive_pid.set_target(inches);

    initialize_func = false;
  }

  double position_diff = odometry->get_position().y - saved_pos.y;

  // printf("Pos diff: %f Saved pos: %f  ", position_diff, saved_pos.y);
  printf("Tank X: %f   Tank Y: %f  ", odometry->get_position().y, odometry->get_position().x);

  // Update PID loop and drive the robot based on it's output
  drive_pid.update(position_diff);

  // Drive backwards if we input negative inches, forward for positive
  drive_tank(drive_pid.get(), drive_pid.get());

  // If the robot is at it's target, return true
  if (drive_pid.is_on_target())
  {
    drive_tank(0, 0);
    initialize_func = true;
    return true;
  }

  return false;
}

/**
 * Autonomously turn the robot X degrees to the right (negative for left), with a maximum motor speed
 * of percent_speed (-1.0 -> 1.0)
 * 
 * Uses a PID loop for it's control.
 */
bool TankDrive::turn_degrees(double degrees, double percent_speed)
{
  // On the first run of the funciton, reset the gyro position and PID
  if (initialize_func)
  {
    saved_pos = odometry->get_position();
    turn_pid.reset();

    turn_pid.set_limits(-fabs(percent_speed), fabs(percent_speed));
    turn_pid.set_target(degrees);

    initialize_func = false;
  }

  // Update PID loop and drive the robot based on it's output
  turn_pid.update(odometry->get_position().rot - saved_pos.rot);
  drive_tank(turn_pid.get(), -turn_pid.get());

  // If the robot is at it's target, return true
  if (turn_pid.is_on_target())
  {
    drive_tank(0, 0);
    initialize_func = true;
    return true;
  }

  return false;
}