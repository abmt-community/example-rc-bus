#ifndef BUS_OPENCV_H
#define BUS_OPENCV_H BUS_OPENCV_H

#include <abmt/img.h>
#include <vector>

namespace bus{

//@link: opencv_core
//@link: opencv_imgproc

//@node: auto
struct blur {
    abmt::img_gray in_img;
    int            in_blur_size = 10;
    abmt::img_gray out;
    
    void tick();
};


//@node: auto
struct canny {
    abmt::img_gray in_img;
    double         in_threshold1 = 50;
    double         in_threshold2 = 50;
    abmt::img_gray out;

    void tick();
};

//@node: auto
struct hough_lines {
    abmt::img_gray            in;
    std::vector<abmt::line2d> out;
    
    void tick();
};



} // namespace bus

#endif // BUS_OPENCV_H
