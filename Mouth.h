#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
using namespace cv;
using namespace std; 

class Mouth
{
public:
    Mouth(Rect mouthRect);
    Mouth(vector<Point2f> points);
    ~Mouth();
    
    Rect                getRect();
    vector<Point2f>     getPoints();
private:
    vector<Point2f> m_points;
    Rect            *m_rect;
};

