#include "main.h"
#include "../../include/RAJA/controllers/Closed-Loop-Controller.h"

ClosedLoop::ClosedLoop(float kp, float ki, float kd, float ls, char state):
                       kp(kp), ki(ki), kd(kd), ls(ls), control_mode(state){}


void ClosedLoop::set_constants(float _kp, float _kd, float _ki, float _ls){
    kp = _kp; 
    ki = _ki; 
    kd = _kd; 
    ls = _ls; 
}

int ClosedLoop::sign(int x){
    if(x>0) return 1; 
    else return -1;
}

void ClosedLoop::limit_acceleration(bool state, int max_accel){
    using_slew_rate = state; 
    accel_step = max_accel;
}

float ClosedLoop::get_calculated_power(int set_point, float sensor_value){
    int error = set_point - sensor_value; 
    p_power = kp * error; 
    pros::delay(50); 

    if(abs(error) < i_bound && abs(error) > INTERGAL_ERROR_TOLERANCE){
        integral += error; 
    }
    else{
        integral = 0; 
    }
    i_power = ki * integral; 

    int delta_error = (set_point - sensor_value) - error; 
    d_power = kd * delta_error; 

    if(using_slew_rate && abs(delta_error) > accel_step){
        power += (sign(p_power) * ls); 
    }
    else{
        power = p_power + i_power + d_power; 
    }

    return power; 
}

