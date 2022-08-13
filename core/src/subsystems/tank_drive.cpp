#include "../core/include/subsystems/tank_drive.h"

TankDrive::TankDrive(motor_group &left_motors, motor_group &right_motors, robot_specs_t &config, OdometryTank *odom)
    : left_motors(left_motors), right_motors(right_motors),
     correction_pid(config.correction_pid), odometry(odom), config(config)
{

}

/**
 * Reset the initialization for autonomous drive functions
 */
void TankDrive::reset_auto()
{
  func_initialized = false;
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
void TankDrive::drive_tank(double left, double right, int power, bool isdriver)
{
  left = modify_inputs(left, power);
  right = modify_inputs(right, power);

  if(isdriver == false)
  {
    left_motors.spin(directionType::fwd, left * 12, voltageUnits::volt);
    right_motors.spin(directionType::fwd, right * 12, voltageUnits::volt);
  }else
  {
    left_motors.spin(directionType::fwd, left * 100.0, percentUnits::pct);
    right_motors.spin(directionType::fwd, right * 100.0, percentUnits::pct);
  }
}

/**
 * Drive the robot using arcade style controls. forward_back controls the linear motion,
 * left_right controls the turning.
 * 
 * left_motors and right_motors are in "percent": -1.0 -> 1.0
 */
void TankDrive::drive_arcade(double forward_back, double left_right, int power)
{
  forward_back = modify_inputs(forward_back, power);
  left_right = modify_inputs(left_right, power);

  double left = forward_back + left_right;
  double right = forward_back - left_right;

  left_motors.spin(directionType::fwd, left * 12, voltageUnits::volt);
  right_motors.spin(directionType::fwd, right * 12, voltageUnits::volt);
}

/**
 * Autonomously drive forward or backwards, X inches infront or behind the robot's current position.
 * This driving method is relative, so excessive use may cause the robot to get off course!
 *
 * @param inches Distance to drive in a straight line
 * @param speed How fast the robot should travel, 0 -> 1.0
 * @param correction How much the robot should correct for being off angle
 * @param dir Whether the robot is travelling forwards or backwards
 */
bool TankDrive::drive_forward(double inches, MotionController &motion, directionType dir)
{
  // We can't run the auto drive function without odometry
  if(odometry == NULL)
  {
    fprintf(stderr, "Odometry is NULL. Unable to run drive_forward()\n");
    fflush(stderr);
    return true;
  }

  static position_t start_pos;
  
  // Decide whether to go forwards or backwards
  int dir_sign = (dir == directionType::rev) ? -1 : 1;
  inches *= dir_sign;

  // On the first run of the function, initialize the motion profile and correction w/ a target
  if (!func_initialized)
  {
    start_pos = odometry->get_position();

    motion.init(0, inches);    
    correction_pid.set_target(0);
    correction_pid.reset();

    func_initialized = true;
  }

  // Update the motion profile with the distance from the start point
  position_t cur_pos = odometry->get_position();
  correction_pid.update(OdometryBase::rot_diff(cur_pos, start_pos));

  double out = motion.update(dir_sign * OdometryBase::pos_diff(odometry->get_position(), start_pos));
  double correction = correction_pid.get();

  drive_tank(out - correction, out + correction, 1, false);

  // Function end: reset for the next run
  if (motion.is_on_target())
  {
    stop();
    func_initialized = false;
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
bool TankDrive::turn_degrees(double degrees, MotionController &motion)
{
  // We can't run the auto drive function without odometry
  if(odometry == NULL)
  {
    fprintf(stderr, "Odometry is NULL. Unable to run drive_forward()\n");
    fflush(stderr);
    return true;
  }

  static position_t start_pos;

  // On the first run of the funciton, reset the gyro position and PID
  if (!func_initialized)
  {
    start_pos = odometry->get_position();
    motion.init(-degrees, 0);

    func_initialized = true;
  }
  
  double heading = odometry->get_position().rot - start_pos.rot;
  double delta_heading = OdometryBase::smallest_angle(heading, degrees);
  double out = motion.update(delta_heading);

  // printf("heading: %f, delta_heading: %f\n", heading, delta_heading);

  drive_tank(out, -out);

  // If the robot is at it's target, return true
  if (motion.is_on_target())
  {
    drive_tank(0, 0);
    func_initialized = false;
    return true;
  }

  return false;
}

/**
  * Use odometry to automatically drive the robot to a point on the field.
  * X and Y is the final point we want the robot.
  *
  * Returns whether or not the robot has reached it's destination.
  */
bool TankDrive::drive_to_point(double x, double y, MotionController &motion, vex::directionType dir)
{
  // We can't run the auto drive function without odometry
  if(odometry == NULL)
  {
    fprintf(stderr, "Odometry is NULL. Unable to run drive_forward()\n");
    fflush(stderr);
    return true;
  }


  
  if(!func_initialized)
  {
    // Reset the control loops
    correction_pid.reset();

    double initial_dist = OdometryBase::pos_diff(odometry->get_position(), {.x=x, .y=y});
    if (dir == directionType::rev)
      initial_dist *= -1;

    motion.init(-initial_dist, 0);

    correction_pid.set_limits(-1, 1);
    correction_pid.set_target(0);

    // point_orientation_deg = atan2(y - odometry->get_position().y, x - odometry->get_position().x) * 180.0 / PI;

    func_initialized = true;
  }

  // Store the initial position of the robot
  position_t current_pos = odometry->get_position();
  position_t end_pos = {.x=x, .y=y};

  // Create a point (and vector) to get the direction
  Vector::point_t pos_diff_pt = 
  {
    .x = x - current_pos.x,
    .y = y - current_pos.y
  };

  Vector point_vec(pos_diff_pt);

  // Get the distance between 2 points
  double dist_left = OdometryBase::pos_diff(current_pos, end_pos);
  
  int sign = 1;

  // Make an imaginary perpendicualar line to that between the bot and the point. If the point is behind that line,
  // and the point is within the robot's radius, use negatives for feedback control.

  double angle_to_point = atan2(y - current_pos.y, x - current_pos.x) * 180.0 / PI;
  double angle = fmod(current_pos.rot - angle_to_point, 360.0);
  // Normalize the angle between 0 and 360
  if (angle > 360) angle -= 360;
  if (angle < 0) angle += 360; 
  // If the angle is behind the robot, report negative.
  if (dir == directionType::fwd && angle > 90 && angle < 270)
    sign = -1;
  else if(dir == directionType::rev && (angle < 90 || angle > 270))
    sign = -1;

  if (fabs(dist_left) < config.drive_correction_cutoff) 
  {
    // When inside the robot's cutoff radius, report the distance to the point along the robot's forward axis,
    // so we always "reach" the point without having to do a lateral translation
    dist_left *= fabs(cos(angle * PI / 180.0));
  }

  // Get the heading difference between where we are and where we want to be
  // Optimize that heading so we don't turn clockwise all the time
  double heading = rad2deg(point_vec.get_dir());
  double delta_heading = 0;

  // Going backwards "flips" the robot's current heading
  if (dir == directionType::fwd)
    delta_heading = OdometryBase::smallest_angle(current_pos.rot, heading);
  else
    delta_heading = OdometryBase::smallest_angle(current_pos.rot - 180, heading);

  // Update the PID controllers with new information
  correction_pid.update(delta_heading);
  double motion_rval = motion.update(sign * -1 * dist_left);

  // Disable correction when we're close enough to the point
  double correction = 0;
  if(is_pure_pursuit || fabs(dist_left) > config.drive_correction_cutoff)
    correction = correction_pid.get();

  // Reverse the drive_pid output if we're going backwards
  if(dir == directionType::rev)
     motion_rval *= -1;

  // Combine the two pid outputs
  double lside = motion_rval + correction;
  double rside = motion_rval - correction;

  // limit the outputs between -1 and +1
  lside = (lside > 1) ? 1 : (lside < -1) ? -1 : lside;
  rside = (rside > 1) ? 1 : (rside < -1) ? -1 : rside;

  drive_tank(lside, rside, 1, false);

  printf("dist: %f\n", sign * -1 * dist_left);
  fflush(stdout);

  // Check if the robot has reached it's destination
  if(motion.is_on_target())
  {
    stop();
    func_initialized = false;
    return true;
  }

  return false;
}

/**
 * Turn the robot in place to an exact heading relative to the field.
 * 0 is forward, and 0->360 is clockwise.
 */
bool TankDrive::turn_to_heading(double heading_deg, MotionController &motion)
{
  // We can't run the auto drive function without odometry
  if(odometry == NULL)
  {
    fprintf(stderr, "Odometry is NULL. Unable to run drive_forward()\n");
    fflush(stderr);
    return true;
  }

  if(!func_initialized)
  {
    double initial_diff = OdometryBase::smallest_angle(odometry->get_position().rot, heading_deg);
    motion.init(-initial_diff, 0);

    func_initialized = true;
  }

  // Get the difference between the new heading and the current, and decide whether to turn left or right.
  double delta_heading = OdometryBase::smallest_angle(odometry->get_position().rot, heading_deg);
  double rval = motion.update(delta_heading);

  printf("~TURN~ delta: %f\n", delta_heading);
  // printf("delta heading: %f, pid: %f\n", delta_heading, turn_pid.get());
  fflush(stdout);

  drive_tank(rval, -rval);

  // When the robot has reached it's angle, return true.
  if(motion.is_on_target())
  {
    func_initialized = false;
    stop();
    return true;
  }

  return false;
}

/**
 * Modify the inputs from the controller by squaring / cubing, etc
 * Allows for better control of the robot at slower speeds
 */
double TankDrive::modify_inputs(double input, int power)
{
  return (power % 2 == 0 ? (input < 0 ? -1 : 1) : 1) * pow(input, power);
}

bool TankDrive::pure_pursuit(std::vector<PurePursuit::hermite_point> path, double radius, MotionController &motion, double res, directionType dir) {
  is_pure_pursuit = true;
  std::vector<Vector::point_t> smoothed_path = PurePursuit::smooth_path_hermite(path, res);

  Vector::point_t lookahead = PurePursuit::get_lookahead(smoothed_path, {odometry->get_position().x, odometry->get_position().y}, radius);
  //printf("%f\t%f\n", odometry->get_position().x, odometry->get_position().y); 
  //printf("%f\t%f\n", lookahead.x, lookahead.y);
  bool is_last_point = (path.back().x == lookahead.x) && (path.back().y == lookahead.y);

  if(is_last_point)
    is_pure_pursuit = false;

  bool retval = drive_to_point(lookahead.x, lookahead.y, motion, dir);

  if(is_last_point)
    return retval;

  return false;
}
