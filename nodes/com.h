#ifndef BUS_COM_H
#define BUS_COM_H BUS_COM_H

#include <abmt/img.h>
#include <abmt/time.h>
#include <io_utils/mbu.h>
#include <memory>
#include <netinet/in.h>

namespace bus{

//@node: auto
//@raster: auto
struct udp_send{
    
    float in_pwm1 = 0;
    float in_pwm2 = 0;
    float in_pwm3 = 0;
    float in_pwm4 = 0;
    float in_red   = 0;
    float in_flash = 0;
    
    int fd = 0;
    sockaddr_in target;
    
    void init();
    void tick();
    void final();
    
    ~udp_send();
};


//@node: auto
//@raster: source
struct udp_rcv{
    abmt::img out_img;
    
    int fd = 0;
    char data[64*1024];
    
    void init();
    bool poll();
    void final();
    
    ~udp_rcv();
};



} // namespace bus

#endif // BUS_COM_H
