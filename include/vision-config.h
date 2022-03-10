/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature YELLOW_MOGO = vex::vision::signature (1, 1535, 2055, 1795, -3835, -3647, -3741, 11, 0);
vex::vision::signature BLUE_MOGO = vex::vision::signature (2, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature RED_MOGO = vex::vision::signature (3, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::code ANY_MOGO = vex::vision::code(YELLOW_MOGO, BLUE_MOGO, RED_MOGO);

vex::vision cam = vex::vision (vex::PORT9, 50, YELLOW_MOGO, BLUE_MOGO, RED_MOGO);
/*vex-vision-config:end*/