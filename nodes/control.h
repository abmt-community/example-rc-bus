#ifndef BUS_CONTROL_H
#define BUS_CONTROL_H BUS_CONTROL_H

#include <vector>
#include <abmt/img.h>
#include <abmt/man_ctrl.h>

namespace bus{

//@node:auto
//@raster: 20ms
struct manual_drive{
    abmt::man_ctrl_state in;
    
    double out_up;
    double out_down;
    double out_left;
    double out_right;
    
    double value_drive = 0.6;
    double value_steer = 1;
    
    
    void tick();
};


//@node: auto
struct calc_delta{
    std::vector<abmt::line2d>   in_lines;
    abmt::img                   in_image;
    double                      in_limit = 3.0/4;
    double                      in_target_height = 100;
    double                      in_lane_with = 100;
    
    abmt::img_rgba  out_img;
    double          out_delta = 0;
    double          out_delta_norm = 0;
    bool            out_lane_ok = false;


    int no_data_timeout = 0;
    
    void tick();
};

//@node: auto
struct controller{
    double in_delta;
    double in_p = 4;
    bool   in_lane_ok = false;
    
    double out_pwm_left;
    double out_pwm_right;
    double out_pwm_drive;
    
    void tick(){
        out_pwm_right = 0;
        out_pwm_left = 0;
        
        if(in_delta > 0){
            out_pwm_right = in_p*in_delta;
        }else{
            out_pwm_left =  in_p*in_delta * -1;
        }
        
        if(in_lane_ok){
            out_pwm_drive = 0.4;
            if(out_pwm_left > 0.6 || out_pwm_right > 0.6){
                out_pwm_drive = 0.53;
            }
        }else{
            out_pwm_drive = 0;
        }
        
    }
    
};



} // namespace bus

#endif // BUS_CONTROL_H
