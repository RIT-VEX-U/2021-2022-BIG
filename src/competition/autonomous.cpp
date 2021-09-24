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
    while (!drive.drive_forward(-5, 1)) {
      task::sleep(50);
    }
    printf("autonomous.cpp: Completed Test Drive\n");
    Command c = Command("Test", [](int arr[]) {
      return false;
    });
    Action a = DriveAction::drive_forward(drive, 5, 1);
    c.set_action(a);
    int i = c.execute_action();
    printf("autonomous.cpp: Got result %i from action\n", i);
    fflush(stdout);
    fflush(stderr);
    break;
  }
  printf("autonomous.cpp: Finished Autonomous\n");

}