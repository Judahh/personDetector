#include "Nose.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Nose::Nose(Rect noseRect){
    m_rect = &noseRect;
    Point2f corner;
    corner.x=(noseRect.x);
    corner.y=(noseRect.y);
    m_points.push_back(corner);

    Point2f corner2;
    corner2.x=(noseRect.x + noseRect.width);
    corner2.y=(noseRect.y);
    m_points.push_back(corner2);

    Point2f corner3;
    corner3.x=(noseRect.x);
    corner3.y=(noseRect.y + noseRect.height);
    m_points.push_back(corner3);

    Point2f corner4;
    corner4.x=(noseRect.x + noseRect.width);
    corner4.y=(noseRect.y + noseRect.height);
    m_points.push_back(corner4);
}

Nose::Nose(vector<Point2f> points){
    
}

Nose::~Nose(){
    // if (m_NoseCascade != NULL) {
    //     delete m_NoseCascade;
    // }
}

vector<Point2f> Nose::getPoints(){
    return m_points;
}

Rect Nose::getRect(){
    return *m_rect;
}