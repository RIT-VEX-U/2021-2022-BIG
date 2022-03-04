#include "automation.h"
#include "vision-config.h"

bool automation::drive_to_goal(double speed, bool (*end_condition) (void))
{
  // PID tuning only used here
  static PID::pid_config_t pid_cfg = {
    .p = .003
  };
  static PID pid(pid_cfg);

  // Take an image, process it and find how far it is from the center
  cam.takeSnapshot(YELLOW_MOGO);
  vision::object blob = cam.largestObject;

  // Filter out small blobs
  if (blob.width * blob.height > 1000)
  {
    double dist_from_center = blob.centerX - (320/2); // pixels
    pid.update(dist_from_center);

    drive.drive_tank(speed - pid.get(), speed + pid.get());

    printf("dist_from_center %f\n", dist_from_center);
  }

  
  if(end_condition())
  {
    drive.stop();
    return true;
  }

  return false;
}