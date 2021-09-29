#include "competition/autonomous.h"
#include "../core/include/utils/command.h"
#include "../core/include/utils/action.h"
#include "robot-config.h"

/**
 * Contains all the code run during autonomous.
 */ 
void Autonomous::autonomous() 
{
  // ========== INIT ==========
  
  printf("autonomous.cpp: Starting Autonomous\n");
  fflush(stdout);
  // ========== MAIN LOOP ==========
  while(true)
  {
    printf("autonomous.cpp: Running\n");
    printf("autonomous.cpp: Starting Test Drive\n");
    while (!drive.drive_forward(-5, 1)) {
      task::sleep(20);
    }
    printf("autonomous.cpp: Completed Test Drive\n");
    Command c("Test1");
    Action a = DriveAction::drive_forward(drive, 5, 1);
    c.set_action(a);
    fflush(stdout);
    
    //BEGIN UNTESTED CHANGES
    Command c2("Test2");
    //Action a2 = DriveAction::turn_cw(drive, 360, 1);
    //c2.set_action(a2);
    //c.add_next(c2);
    //END UNTESTED CHANGES

    int i = c.execute_action();
    printf("autonomous.cpp: Got result %i from command\n", i);
    fflush(stdout);
    fflush(stderr);
    break;
  }
  printf("autonomous.cpp: Finished Autonomous\n");

}