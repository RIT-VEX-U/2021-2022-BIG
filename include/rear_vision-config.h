/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature R_YELLOW_GOAL = vex::vision::signature (1, 1257, 3171, 2214, -3381, -2999, -3190, 2.5, 0);
vex::vision::signature R_RED_GOAL = vex::vision::signature (2, 7171, 9279, 8225, -579, -231, -405, 4.7, 0);
vex::vision::signature R_BLUE_GOAL = vex::vision::signature (3, -1049, -489, -769, 5917, 6931, 6424, 3.9, 0);
vex::vision::signature R_SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature R_SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature R_SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature R_SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision rear_cam = vex::vision (vex::PORT2, 37, R_YELLOW_GOAL, R_RED_GOAL, R_BLUE_GOAL, R_SIG_4, R_SIG_5, R_SIG_6, R_SIG_7);
/*vex-vision-config:end*/