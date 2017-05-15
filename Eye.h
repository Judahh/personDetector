#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
using namespace cv;
using namespace std; 

class Eye
{
public:
    Eye(Rect eyeRect);
    Eye(vector<Point2f> points);
    ~Eye();

    Rect                getRect();
    vector<Point2f>     getPoints();
private:
    vector<Point2f> m_points;
    Rect            *m_rect;
};

