#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
using namespace cv;
using namespace std; 

class Nose
{
public:
    Nose(Rect noseRect);
    Nose(vector<Point2f> points);
    ~Nose();
    
    Rect                getRect();
    vector<Point2f>     getPoints();
private:
    vector<Point2f> m_points;
    Rect            *m_rect;
};

