#ifndef _ODOMETRY_TANK_
#define _ODOMETRY_TANK_

#include "../core/include/subsystems/odometry/odometry_base.h"
#include "../core/include/utils/vector.h"
#include "../core/include/robot_specs.h"

static int background_task(void* odom_obj);

class OdometryTank : public OdometryBase
{
public:
    /**
     * Initialize the Odometry module, using the IMU to get rotation
     * @param left_side The left motors 
     * @param right_
     * side The right motors
     * @param imu The robot's inertial sensor
     * @param is_async If true, the robot will automatically poll it's position and update it in the background.
     *      If false, the update() function must be called periodically.
     */
    OdometryTank(vex::motor_group &left_side, vex::motor_group &right_side, vex::inertial &imu, robot_specs_t &config, bool is_async=true);

    /**
     * Initialize the Odometry module, calculating the rotation from encoders
     * @param left_side The left motors 
     * @param right_side The right motors
     * @param imu The robot's inertial sensor
     * @param is_async If true, the robot will automatically poll it's position and update it in the background.
     *      If false, the update() function must be called periodically.
     */
    OdometryTank(vex::motor_group &left_side, vex::motor_group &right_side, robot_specs_t &config, bool is_async=true);

    /**
     * Update the current position on the field based on the sensors
     */
    position_t update() override;


    void set_position(const position_t &newpos=zero_pos) override;

private:
    /**
     * Get information from the input hardware and an existing position, and calculate a new current position
     */
    static position_t calculate_new_pos(robot_specs_t &config, position_t &stored_info, double lside_diff, double rside_diff, double angle_deg);

    vex::motor_group &left_side, &right_side;
    vex::inertial *imu;
    robot_specs_t &config;

    double rotation_offset = 0;
    
};

#endif