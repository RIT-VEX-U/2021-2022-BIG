/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature R_YELLOW_GOAL = vex::vision::signature (1, 1613, 2389, 2002, -3923, -3355, -3638, 5, 0);
vex::vision::signature R_RED_GOAL = vex::vision::signature (2, 6275, 8193, 7234, -1725, 203, -761, 6.1, 0);
vex::vision::signature R_BLUE_GOAL = vex::vision::signature (3, -3001, -1921, -2461, 8463, 11161, 9812, 4.5, 0);
vex::vision::signature R_SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature R_SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature R_SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature R_SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision rear_cam = vex::vision (vex::PORT2, 27, R_YELLOW_GOAL, R_RED_GOAL, R_BLUE_GOAL, R_SIG_4, R_SIG_5, R_SIG_6, R_SIG_7);
/*vex-vision-config:end*/