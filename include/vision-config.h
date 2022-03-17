/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature YELLOW_MOGO = vex::vision::signature (1, 47, 2217, 1132, -3699, -2875, -3287, 2.2, 0);
vex::vision::signature RED_MOGO = vex::vision::signature (2, 6205, 7359, 6782, -929, -299, -614, 3.9, 0);
vex::vision::signature BLUE_MOGO = vex::vision::signature (3, -2501, -1401, -1951, 6203, 10083, 8143, 3.1, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision cam = vex::vision (vex::PORT9, 50, YELLOW_MOGO, RED_MOGO, BLUE_MOGO, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/