#include "Mouth.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Mouth::Mouth(Rect mouthRect){
    m_rect = &mouthRect;
    Point2f corner;
    corner.x=(mouthRect.x);
    corner.y=(mouthRect.y);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner2;
    corner.x=(mouthRect.x + mouthRect.width);
    corner.y=(mouthRect.y);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner3;
    corner.x=(mouthRect.x);
    corner.y=(mouthRect.y + mouthRect.height);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }

    // Point2f corner4;
    corner.x=(mouthRect.x + mouthRect.width);
    corner.y=(mouthRect.y + mouthRect.height);
    if(corner.x>=0 && corner.y>=0){
        m_points.push_back(corner);
    }
}

Mouth::Mouth(vector<Point2f> points){
    
}

Mouth::~Mouth(){
    // if (m_MouthCascade != NULL) {
    //     delete m_MouthCascade;
    // }
}

vector<Point2f> Mouth::getPoints(){
    return m_points;
}

Rect Mouth::getRect(){
    return *m_rect;
}