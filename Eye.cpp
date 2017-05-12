#include "Eye.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Eye::Eye(Rect eyeRect){
    this->rect = &eyeRect;
    Point2f corner;
    corner.x=(eyeRect.x);
    corner.y=(eyeRect.y);
    this->points.push_back(corner);

    Point2f corner2;
    corner2.x=(eyeRect.x + eyeRect.width);
    corner2.y=(eyeRect.y);
    this->points.push_back(corner2);

    Point2f corner3;
    corner3.x=(eyeRect.x);
    corner3.y=(eyeRect.y + eyeRect.height);
    this->points.push_back(corner3);

    Point2f corner4;
    corner4.x=(eyeRect.x + eyeRect.width);
    corner4.y=(eyeRect.y + eyeRect.height);
    this->points.push_back(corner4);
}

Eye::Eye(vector<Point2f> points){
    
}

Eye::~Eye(){
    // if (m_EyeCascade != NULL) {
    //     delete m_EyeCascade;
    // }
}