#include "Eye.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Eye::Eye(Rect eyeRect){
    m_rect = &eyeRect;
    Point2f corner;
    corner.x=(eyeRect.x);
    corner.y=(eyeRect.y);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner2;
    corner.x=(eyeRect.x + eyeRect.width);
    corner.y=(eyeRect.y);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner3;
    corner.x=(eyeRect.x);
    corner.y=(eyeRect.y + eyeRect.height);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner4;
    corner.x=(eyeRect.x + eyeRect.width);
    corner.y=(eyeRect.y + eyeRect.height);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }
}

Eye::Eye(vector<Point2f> points){
    
}

Eye::~Eye(){
    // if (m_EyeCascade != NULL) {
    //     delete m_EyeCascade;
    // }
}

vector<Point2f> Eye::getPoints(){
    return m_points;
}

Rect Eye::getRect(){
    return *m_rect;
}