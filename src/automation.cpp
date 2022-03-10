#include "automation.h"
#include "vision-config.h"

using namespace automation;

double vision_x_dist(GoalType color)
{
  // Take an image, process it and find how far it is from the center
  switch (color)
  {
    case YELLOW: 
      cam.takeSnapshot(YELLOW_MOGO);
      break;
    case BLUE:
      cam.takeSnapshot(BLUE_MOGO);
      break;
    case RED:
      cam.takeSnapshot(RED_MOGO);
    case ANY:
      cam.takeSnapshot(ANY_MOGO);
  }

  vision::object blob = cam.largestObject;
  double dist_from_center = 0;

  // Filter out small blobs
  if (blob.width * blob.height > 1000)
    dist_from_center = blob.centerX - (320/2); // pixels

  return dist_from_center;
}

bool automation::drive_to_goal(double speed, bool (*end_condition) (void), GoalType color)
{
  // PID tuning only used here
  static PID::pid_config_t pid_cfg = {
    .p = .003
  };
  static PID pid(pid_cfg);

  pid.update(vision_x_dist(color));

  drive.drive_tank(speed - pid.get(), speed + pid.get());

  if(end_condition())
  {
    drive.stop();
    return true;
  }

  return false;
}

void automation::drive_with_autoaim(double left, double right, int power, GoalType color)
{
  static PID::pid_config_t pid_cfg = {
    .p = .002
  };
  static PID pid(pid_cfg);

  pid.update(vision_x_dist(color));
  drive.drive_tank(left - pid.get(), right - pid.get(), power);
}