/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature YELLOW_MOGO = vex::vision::signature (1, -1, 3121, 1560, -4759, -4325, -4542, 4.7, 0);
vex::vision::signature RED_MOGO = vex::vision::signature (2, 7329, 8231, 7780, -1821, -1521, -1671, 6.6, 0);
vex::vision::signature BLUE_MOGO = vex::vision::signature (3, -1655, -1033, -1344, 4725, 5891, 5308, 4.3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision cam = vex::vision (vex::PORT4, 44, YELLOW_MOGO, RED_MOGO, BLUE_MOGO, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/