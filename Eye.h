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
    
    vector<Point2f> points;
    Rect            *rect;
};

