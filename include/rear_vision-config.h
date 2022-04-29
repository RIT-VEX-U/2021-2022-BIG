/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature R_YELLOW_GOAL = vex::vision::signature (1, 97, 3963, 2030, -4283, -3779, -4030, 2.5, 0);
vex::vision::signature R_RED_GOAL = vex::vision::signature (2, 7509, 9203, 8356, -1185, -471, -828, 4.7, 0);
vex::vision::signature R_BLUE_GOAL = vex::vision::signature (3, -2591, -1681, -2136, 8241, 12385, 10314, 3.9, 0);
vex::vision::signature R_SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature R_SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature R_SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature R_SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision rear_cam = vex::vision (vex::PORT2, 50, R_YELLOW_GOAL, R_RED_GOAL, R_BLUE_GOAL, R_SIG_4, R_SIG_5, R_SIG_6, R_SIG_7);
/*vex-vision-config:end*/