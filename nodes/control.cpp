#include "control.h"

using namespace std;
using namespace bus;

using namespace abmt;

void manual_drive::tick(){
    out_up    = 0;
    out_down  = 0;
    out_right = 0;
    out_left  = 0;

    if(in.key_pressed(38)){
        out_up = value_drive;
        if(in.shift){
            out_up = out_up * 2;
        }
    }
    
    if(in.key_pressed(40)){
        out_down = value_drive;
    }
    
    if(in.key_pressed(39)){
        out_right = value_steer;
    }
     
    if(in.key_pressed(37)){
        out_left = value_steer;
    }
}

void calc_delta::tick(){

    out_img = in_image.copy();

    double new_delta_top = 0;
    int    num_deltas = 0;
    
    abmt::line2d ctl_top_line = {{(double)0,(double)in_target_height}, {(double)out_img.width, (double)in_target_height}}; 
    out_img.draw_line(ctl_top_line, {0,0,255,255}, 2);

    for(auto l: in_lines){
        // swap that p1 is at bottom
        if(l.p2.y < l.p1.y){
            auto tmp = l.p2;
            l.p2 = l.p1;
            l.p1 = tmp;
        }
        if( ( l.p1.y > ctl_top_line.p1.y || l.p2.y > ctl_top_line.p1.y ) && l.len() > 0.2 ){
    
            auto r = abmt::ray<2,double>::from_2p(l.p1, l.p2);
            // Horizontale linien rausschmeißen
            if(abs(r.dir.y) < 0.2){
                continue;
            }
            
            auto p_side_check = r.g(l.len()/2);
            
            vec2 p_l = {p_side_check.x - 10, p_side_check.y};
            vec2 p_r = {p_side_check.x + 10, p_side_check.y};
            
            if(p_r.x >= out_img.width){
                continue;
            }
            if(p_l.x < 0){
                continue;
            }
            
            auto get_col = [&](vec2 p)->double{
                double ret = 0;
                for(auto point: out_img.roi(p.x-2, p.y-2, 4,4)){
                    ret += point.color.quad_len();
                }
                return ret;
            };
            
            int f_t_offset = 1;
            if(get_col(p_l) < get_col(p_r)){
                out_img.draw_line(l, {255,0,0,255}, 3);
                f_t_offset = -1;
            }else{
                out_img.draw_line(l, {0,255,0,255}, 3);
            }
            
            double g_top_corss = (ctl_top_line.p1.y - r.p.y ) / r.dir.y;
            auto  cp_top = r.g(g_top_corss); // cp = cross point
            cp_top.x += in_lane_with/2 * f_t_offset;
            out_img.draw_cross(cp_top, {255,255,255,255});
            
            new_delta_top += cp_top.x - out_img.width/2;
            num_deltas++;
            
           
        }else{
            out_img.draw_line(l, {0,0,0, 255}, 1);
        }
    }
    if(num_deltas > 0){
        out_delta = new_delta_top / num_deltas;
        no_data_timeout = 0;
        out_lane_ok = true;
    }else{
        no_data_timeout++;
    }
    if(no_data_timeout > 10){
        out_delta = 0;
        out_lane_ok = false;
    }
    out_delta_norm = out_delta / (in_image.width / 2);
}