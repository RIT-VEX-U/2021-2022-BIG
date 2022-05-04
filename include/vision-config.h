/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature YELLOW_MOGO = vex::vision::signature (1, 45, 1213, 630, -4907, -3839, -4372, 2.5, 0);
vex::vision::signature RED_MOGO = vex::vision::signature (2, 4805, 5739, 5272, -1771, 937, -416, 8.6, 0);
vex::vision::signature BLUE_MOGO = vex::vision::signature (3, -3641, -2573, -3107, 8453, 11031, 9742, 3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision cam = vex::vision (vex::PORT4, 44, YELLOW_MOGO, RED_MOGO, BLUE_MOGO, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/