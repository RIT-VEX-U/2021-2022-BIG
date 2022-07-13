#pragma once

typedef struct
{
    double pos;
    double vel;
    double accel;

} motion_t;

class MotionProfile
{
    public:
    virtual motion_t calculate(double time_s)=0;
    virtual void set_endpts(double start, double end)=0;
};
