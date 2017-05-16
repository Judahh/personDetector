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
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner2;
    corner.x=(noseRect.x + noseRect.width);
    corner.y=(noseRect.y);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner3;
    corner.x=(noseRect.x);
    corner.y=(noseRect.y + noseRect.height);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner4;
    corner.x=(noseRect.x + noseRect.width);
    corner.y=(noseRect.y + noseRect.height);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }
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