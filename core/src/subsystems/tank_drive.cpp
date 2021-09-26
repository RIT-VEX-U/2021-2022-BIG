#include "../core/include/subsystems/tank_drive.h"

TankDrive::TankDrive(motor_group &left_motors, motor_group &right_motors, inertial &gyro_sensor, TankDrive::tankdrive_config_t &config, OdometryTank *odom)
    : left_motors(left_motors), right_motors(right_motors),
     drive_pid(config.drive_pid), turn_pid(config.turn_pid), correction_pid(config.correction_pid), odometry(odom)
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
void TankDrive::drive_tank(double left, double right, int power)
{
  left = modify_inputs(left, power);
  right = modify_inputs(right, power);

  left_motors.spin(directionType::fwd, left * 12, voltageUnits::volt);
  right_motors.spin(directionType::fwd, right * 12, voltageUnits::volt);
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
  if (!func_initialized)
  {
    saved_pos = odometry->get_position();
    drive_pid.reset();

    drive_pid.set_limits(-fabs(percent_speed), fabs(percent_speed));
    drive_pid.set_target(inches);

    func_initialized = true;
  }

  double position_diff = odometry->get_position().y - saved_pos.y;

  // Update PID loop and drive the robot based on it's output
  drive_pid.update(position_diff);

  // Drive backwards if we input negative inches, forward for positive
  drive_tank(drive_pid.get(), drive_pid.get());

  // If the robot is at it's target, return true
  if (drive_pid.is_on_target())
  {
    drive_tank(0, 0);
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
bool TankDrive::turn_degrees(double degrees, double percent_speed)
{
  // On the first run of the funciton, reset the gyro position and PID
  if (!func_initialized)
  {
    saved_pos = odometry->get_position();
    turn_pid.reset();

    turn_pid.set_limits(-fabs(percent_speed), fabs(percent_speed));
    turn_pid.set_target(0);

    func_initialized = true;
  }
  double heading = odometry->get_position().rot - saved_pos.rot;
  double delta_heading = OdometryBase::smallest_angle(heading, degrees);
  turn_pid.update(delta_heading);

  printf("heading: %f, delta_heading: %f\n", heading, delta_heading);

  drive_tank(-turn_pid.get(), turn_pid.get());

  // If the robot is at it's target, return true
  if (turn_pid.is_on_target())
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
bool TankDrive::drive_to_point(double x, double y, double speed, double correction_speed)
{
  if(!func_initialized)
  {
    // Reset the control loops
    drive_pid.reset();
    correction_pid.reset();

    drive_pid.set_limits(-fabs(speed), fabs(speed));
    correction_pid.set_limits(-fabs(correction_speed), fabs(correction_speed));

    // Set the targets to 0, because we update with the change in distance and angle between the current point
    // and the new point.
    drive_pid.set_target(0);
    correction_pid.set_target(0);

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
  double dist_left = -OdometryBase::pos_diff(current_pos, end_pos, true);

  // Get the heading difference between where we are and where we want to be
  // Optimize that heading so we don't turn clockwise all the time
  double heading = rad2deg(point_vec.get_dir());
  double delta_heading = -OdometryBase::smallest_angle(current_pos.rot, heading);

  // Update the PID controllers with new information
  correction_pid.update(delta_heading);
  drive_pid.update(dist_left);

  // Combine the two pid outputs
  double lside = drive_pid.get() + correction_pid.get();
  double rside = drive_pid.get() - correction_pid.get();

  // limit the outputs between -1 and +1
  lside = (lside > 1) ? 1 : (lside < -1) ? -1 : lside;
  rside = (rside > 1) ? 1 : (rside < -1) ? -1 : rside;

  drive_tank(lside, rside);

  // Check if the robot has reached it's destination
  if(drive_pid.is_on_target())
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
bool TankDrive::turn_to_heading(double heading_deg, double speed)
{
  static bool initialized = false;
  if(!initialized)
  {
    turn_pid.reset();
    turn_pid.set_limits(-fabs(speed), fabs(speed));

    // Set the target to zero, and the input will be a delta.
    turn_pid.set_target(0);

    initialized = true;
  }

  // Get the difference between the new heading and the current, and decide whether to turn left or right.
  double delta_heading = OdometryBase::smallest_angle(odometry->get_position().rot, heading_deg);
  turn_pid.update(delta_heading);

  printf("delta heading: %f, pid: %f, ", delta_heading, turn_pid.get());

  drive_tank(-turn_pid.get(), turn_pid.get());

  // When the robot has reached it's angle, return true.
  if(turn_pid.is_on_target())
  {
    initialized = false;
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

/**
  * Returns points of the intersections of a line segment and a circle. The line 
  * segment is defined by two points, and the circle is defined by a center and radius.
  */
std::vector<Vector::point_t> TankDrive::line_circle_intersections(Vector::point_t center, double r, Vector::point_t point1, Vector::point_t point2)
{
  std::vector<Vector::point_t> intersections = {};

  //Do future calculations relative to the circle's center
  point1.y -= center.y;
  point1.x -= center.x;
  point2.y -= center.y;
  point2.x -= center.x;

  double x1, x2, y1, y2;
  //Handling an infinite slope using mx+b and x^2 + y^2 = r^2
  if(point1.x - point2.x == 0)
  {
    x1 = point1.x;
    y1 = sqrt(pow(r, 2) - pow(x1, 2));
    x2 = point1.x;
    y2 = -sqrt(pow(r, 2) - pow(x2, 2));
  }
  //Non-infinite slope using mx+b and x^2 + y^2 = r^2
  else
  {
    double m = (point1.y - point2.y) / (point1.x - point2.x);
    double b = point1.y - (m * point1.x);

    x1 = ((-m * b) + sqrt(pow(r, 2) + (pow(m, 2) * pow(r, 2)) - pow(b, 2))) / (1 + pow(m,2));
    y1 = m * x1 + b;
    x2 = ((-m * b) - sqrt(pow(r, 2) + (pow(m, 2) * pow(r, 2)) - pow(b, 2))) / (1 + pow(m,2));
    y2 = m * x2 + b;
  }

  //The equations used define an infinitely long line, so we check if the detected intersection falls on the line segment.
  if(x1 >= fmin(point1.x, point2.x) && x1 <= fmax(point1.x, point2.x) && y1 >= fmin(point1.y, point2.y) && y1 <= fmax(point1.y, point2.y))
  {
    intersections.push_back(Vector::point_t{.x = x1 + center.x, .y = y1 + center.y});
  }

  if(x2 >= fmin(point1.x, point2.x) && x2 <= fmax(point1.x, point2.x) && y2 >= fmin(point1.y, point2.y) && y2 <= fmax(point1.y, point2.y))
  {
    intersections.push_back(Vector::point_t{.x = x2 + center.x, .y = y2 + center.y});
  }

  return intersections;
}

/**
 * Selects a look ahead from all the intersections in the path.
 */
Vector::point_t TankDrive::get_lookahead(std::vector<Vector::point_t> path, Vector::point_t robot_loc, double radius)
{ 
  //Default: the end of the path
  Vector::point_t target = path.back();

  if(target.dist(robot_loc) <= radius)
  {
    return target;
  }

  //Check each line segment of the path for potential targets
  for(int i = 0; i < path.size() - 1; i++)
  {
    Vector::point_t start = path[i];
    Vector::point_t end = path[i+1];

    std::vector<Vector::point_t> intersections = TankDrive::line_circle_intersections(robot_loc, radius, start, end);
    //Choose the intersection that is closest to the end of the line segment
    //This prioritizes the closest intersection to the end of the path
    for(Vector::point_t intersection: intersections)
    {
      if(intersection.dist(end) < target.dist(end))
        target = intersection;
    }
  }

  return target;
}

/**
 Injects points in a path without changing the curvature with a certain spacing.
*/
std::vector<Vector::point_t> TankDrive::inject_path(std::vector<Vector::point_t> path, double spacing)
{
  std::vector<Vector::point_t> new_path;

  //Injecting points for each line segment
  for(int i = 0; i < path.size() - 1; i++)
  {
    Vector::point_t start = path[i];
    Vector::point_t end = path[i+1];

    Vector::point_t diff = end - start;
    Vector vector = Vector(diff);
    
    int num_points = ceil(vector.get_mag() / spacing);

    //This is the vector between each point
    vector = vector.normalize() * spacing;

    for(int j = 0; j < num_points; j++)
    {
      //We take the start point and add additional vectors
      Vector::point_t path_point = (Vector(start) + vector * j).point();
      new_path.push_back(path_point);
    }
  }
  //Adds the last point
  new_path.push_back(path.back());
  return new_path;
}

/**
 * Returns a smoothed path maintaining the start and end of the path.
 *
 * Weight data is how much weight to update the data (alpha)
 * Weight smooth is how much weight to smooth the coordinates (beta)
 * Tolerance is how much change per iteration is necessary to continue iterating.
 *
 * Honestly have no idea if/how this works.
 * https://medium.com/@jaems33/understanding-robot-motion-path-smoothing-5970c8363bc4
*/

std::vector<Vector::point_t> TankDrive::smooth_path(std::vector<Vector::point_t> path, double weight_data, double weight_smooth, double tolerance)
{
  std::vector<Vector::point_t> new_path = path;
  double change = tolerance;
  while(change >= tolerance)
  {
    change = 0;
    for(int i = 1; i < path.size() - 1; i++)
    {
        Vector::point_t x_i = path[i];
        Vector::point_t y_i = new_path[i];
        Vector::point_t y_prev = new_path[i-1];
        Vector::point_t y_next = new_path[i+1];
        
        Vector::point_t y_i_saved = y_i;
        
        y_i.x += weight_data * (x_i.x - y_i.x) + weight_smooth * (y_next.x + y_prev.x - (2 * y_i.x));
        y_i.y += weight_data * (x_i.y - y_i.y) + weight_smooth * (y_next.y + y_prev.y - (2 * y_i.y));
        new_path[i] = y_i;
        
        change += y_i.dist(y_i_saved);
    }
  }
  return new_path;
}

/**
  Drives through a path using pure pursuit.
*/
bool TankDrive::pure_pursuit(std::vector<Vector::point_t> path, Vector::point_t robot_loc, double radius, double speed, double spacing, double weight_data, double weight_smooth)
{
  std::vector<Vector::point_t> injected_path = TankDrive::inject_path(path, spacing);
  std::vector<Vector::point_t> smoothed_path = TankDrive::smooth_path(injected_path, weight_data, weight_smooth, .0001);
  Vector::point_t lookahead = TankDrive::get_lookahead(smoothed_path, robot_loc, radius);

  printf("look ahead x %f y: %f\n", lookahead.x, lookahead.y);
  return this->drive_to_point(lookahead.x, lookahead.y, speed, speed);
}
