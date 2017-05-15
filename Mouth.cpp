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
    m_points.push_back(corner);

    Point2f corner2;
    corner2.x=(mouthRect.x + mouthRect.width);
    corner2.y=(mouthRect.y);
    m_points.push_back(corner2);

    Point2f corner3;
    corner3.x=(mouthRect.x);
    corner3.y=(mouthRect.y + mouthRect.height);
    m_points.push_back(corner3);

    Point2f corner4;
    corner4.x=(mouthRect.x + mouthRect.width);
    corner4.y=(mouthRect.y + mouthRect.height);
    m_points.push_back(corner4);
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