#include "com.h"

#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;
using namespace bus;

void udp_send::init(){
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        throw std::runtime_error("Error opening socket.");
        fd = 0;
    }

    int on = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if( ret != 0 ) {
            throw std::runtime_error("Error setting SO_REUSEADDR.");
    }
    
    target.sin_addr.s_addr = inet_addr("192.168.1.46");
    target.sin_family = AF_INET;
    target.sin_port = htons(3334);
}

void udp_send::tick(){
    float data[6];
    data[0] = in_pwm1;
    data[1] = in_pwm2;
    data[2] = in_pwm3;
    data[3] = in_pwm4;
    data[4] = in_red;
    data[5] = in_flash;

    sendto(fd, &data, sizeof(data), 0, (struct sockaddr *) &target, sizeof(target));
}

void udp_send::final(){
    if(fd){
        close(fd);
    }
    fd = 0;
}

udp_send::~udp_send(){
    if(fd){
        final();
    }
}



void udp_rcv::init(){
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        throw std::runtime_error("Error opening socket.");
        fd = 0;
    }

    int on = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if( ret != 0 ) {
            throw std::runtime_error("Error setting SO_REUSEADDR.");
    }
    
    sockaddr_in target;
    target.sin_addr.s_addr = htonl(INADDR_ANY);
    target.sin_family = AF_INET;
    target.sin_port = htons(3333);
    ret = ::bind(fd, (sockaddr*) &target, sizeof(target));
    if( ret < 0 ) {
            throw std::runtime_error("Error binding socket.");
    }
    
    out_img = abmt::img_rgb(640,480);
}

bool udp_rcv::poll(){
    bool new_img = false;
    abmt::img rcv_img;
    while(true){
        auto res = recv(fd, data, 64*1024,MSG_DONTWAIT);
        if(res > 0){
            rcv_img.type = abmt::image_type::JPEG;
            rcv_img.data.realloc(res,data);
            new_img = true;
        }else{
            break;
        }
    }
    if(new_img){
        abmt::img_rgba conv = rcv_img;
        out_img  = conv.rotate_180();
        return true;
    }else{
        return false;
    }
}

void udp_rcv::final(){
    if(fd){
        close(fd);
    }
    fd = 0;
}

udp_rcv::~udp_rcv(){
    if(fd){
        final();
    }
}

