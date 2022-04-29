#include "automation.h"
#include "vision-config.h"
#include "rear_vision-config.h"
#include "subsystems.h"

using namespace automation;

double vision_x_dist(GoalType color, directionType dir=fwd)
{
  vex::vision &sensor = (dir == fwd) ? cam : rear_cam;

  vision::object objects[3];

  // Take an image, process it and find how far it is from the center
  switch (color)
  {
    case YELLOW: 
      sensor.takeSnapshot(1);
      break;
    case RED:
      sensor.takeSnapshot(2);
      break;
    case BLUE:
      sensor.takeSnapshot(3);
      break;
    case ANY:
      sensor.takeSnapshot(1);
      objects[0] = sensor.largestObject;
      sensor.takeSnapshot(2);
      objects[1] = sensor.largestObject;
      sensor.takeSnapshot(3);
      objects[2] = sensor.largestObject;
      break;
  }

  printf("Num: %ld\n", sensor.objectCount);

  vision::object &blob = sensor.largestObject;

  if(color == ANY)
    for(int i = 0; i < 3; i++)
    {
      printf("Area: %d\n", objects[i].height * objects[i].width);
      if(objects[i].width * objects[i].height > blob.width * blob.height)
        blob = objects[i];

    }

  double dist_from_center = 0;

  // Filter out small blobs
  if (blob.width * blob.height > 800)
    dist_from_center = blob.centerX - (320/2); // pixels, display is 320p across

  printf("x: %f\n", dist_from_center);

  return dist_from_center;
}

bool automation::drive_to_goal(double speed, bool (*end_condition) (void), GoalType color, directionType dir)
{
  // PID tuning only used here
  static PID::pid_config_t pid_cfg = {
    .p = .005,
    .d = .0001
  };
  static PID pid(pid_cfg);

  pid.update(vision_x_dist(color, dir));

  double l_out = (dir == fwd) ? speed - pid.get() : -speed - pid.get();
  double r_out = (dir == fwd) ? speed + pid.get() : -speed + pid.get();
  
  drive.drive_tank(l_out, r_out);

  if(end_condition())
  {
    drive.stop();
    // front_claw::close();
    return true;
  }

  return false;
}

void automation::drive_with_autoaim(double left, double right, int power, GoalType color)
{
  static PID::pid_config_t pid_cfg = {
    .p = .004
  };
  static PID pid(pid_cfg);

  pid.update(vision_x_dist(color));
  drive.drive_tank(left - pid.get() + .5, right + pid.get() + .5, power);
}