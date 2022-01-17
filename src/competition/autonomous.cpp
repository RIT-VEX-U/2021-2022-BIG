#include "competition/autonomous.h"
#include "core.h"
#include "robot-config.h"

/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  // ========== INIT ==========

  //Main auto path
  odom.set_position({.x = 24, .y=14, .rot=90}); // starting pos

  std::vector<PurePursuit::hermite_point> path_to_neutral = {{.x = 24, .y=14, .dir=90, .mag=50},{.x=27, .y=63, .dir=55, .mag=50}};

  std::vector<PurePursuit::hermite_point> path_from_neutral = {{.x=27, .y=63, .dir=55, .mag=50}, {.x=12, .y=24, .dir=90, .mag=50}};

  GenericAuto auto1;
  // auto1.add([](){return drive.pure_pursuit(std::vector<PurePursuit::hermite_point> path, double radius, double speed, double res)});

  // ========== MAIN LOOP ==========
  while(true)
  {

  }

}