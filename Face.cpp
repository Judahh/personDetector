#include "Face.h"
#include "Eye.h"
#include "Nose.h"
#include "Mouth.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Face::Face(Mat faceMat){
    m_mat = faceMat;
    vector<cv::Rect>   allEyes;
    CascadeClassifier eyeCascade("cascade/eye.xml");
    eyeCascade.detectMultiScale(faceMat, allEyes, 1.1, 3, 0,
    Size(faceMat.rows / 50, faceMat.cols / 50),
    Size(faceMat.rows, faceMat.cols));

    for(auto eyeRect : allEyes){
        Eye eye(eyeRect);
        m_eyes.push_back(eye);
    }

    vector<cv::Rect>   allNoses;
    CascadeClassifier noseCascade("cascade/nose.xml");
    noseCascade.detectMultiScale(faceMat, allNoses, 1.1, 3, 0,
    Size(faceMat.rows / 50, faceMat.cols / 50),
    Size(faceMat.rows, faceMat.cols));

    for(auto noseRect : allNoses){
        Nose nose(noseRect);
        m_noses.push_back(nose);
    }

    vector<cv::Rect>   allMouths;
    CascadeClassifier mouthCascade("cascade/mouth.xml");
    mouthCascade.detectMultiScale(faceMat, allMouths, 1.1, 3, 0,
    Size(faceMat.rows / 50, faceMat.cols / 50),
    Size(faceMat.rows, faceMat.cols));

    for(auto mouthRect : allMouths){
        Mouth mouth(mouthRect);
        m_mouths.push_back(mouth);
    }

    m_points=makePoints();
}

Face::Face(vector<Point2f> facePoints, Mat faceMat){
    // vector<Point2f> probableHeights;
    // vector<Point2f> probableWidths;
    // Point2f mainPoint;
    // for(auto facePoint : facePoints){
    //     if(mainPoint==NULL){
    //         mainPoint=facePoint;
    //     }
    //     if(mainPoint.x>facePoint.x && mainPoint.y>facePoint.y){
    //         mainPoint=facePoint;
    //     }
    //     for(auto facePoint2 : facePoints){
    //         if(facePoint.x>facePoint2.x){
    //             probableWidths.push_back(facePoint.x-facePoint2.x);
    //         }

    //         if(facePoint.y>facePoint2.y){
    //             probableHeights.push_back(facePoint2.y-facePoint.y);
    //         }
    //     }
    // }
}

Face::~Face(){
    // if (m_faceCascade != NULL) {
    //     delete m_faceCascade;
    // }
}

Mat Face::getMat(){
    return m_mat;
}

vector<Point2f> Face::getPoints(){
    return m_points;
}

vector<Mouth> Face::getMouths(){
    return m_mouths;
}

vector<Nose> Face::getNoses(){
    return m_noses;
}

vector<Eye> Face::getEyes(){
    return m_eyes;
}

vector<Point2f> Face::makePoints(){
    vector<Point2f> points;
    for(auto eye : m_eyes){
        points.insert(points.end(), eye.points.begin(), eye.points.end());
    }   

    for(auto nose : m_noses){
        points.insert(points.end(), nose.points.begin(), nose.points.end());
    }  

    for(auto mouth : m_mouths){
        points.insert(points.end(), mouth.points.begin(), mouth.points.end());
    }  
    return points;
}

void Face::updateMat(Mat currentMat){
    Mat lastMat = m_mat;
    bool boolupdateMat=true;
    
    // std::cout << "LAST:" << std::endl; 
    // std::cout << "COLS:" << lastMat.cols << std::endl; 
    // std::cout << "ROWS:" << lastMat.rows << std::endl; 
    // std::cout << "NEW:" << std::endl; 
    // std::cout << "COLS:" << currentMat.cols << std::endl; 
    // std::cout << "ROWS:" << currentMat.rows << std::endl; 
    vector<uchar> status;
    vector<float> errors;
    vector<Point2f> currentPoints = m_points;
    // std::cout << "POINTS:" << currentPoints.size() << std::endl; 
    vector<Point2f> newPoints;
    calcOpticalFlowPyrLK(lastMat, currentMat,
                        currentPoints, newPoints,
                        status, errors);

    float highest=0;
    float smallest=100000;
    // int number=0;
    for(auto cStatus : errors){
        if(cStatus>highest){
            highest=cStatus;
        }
        if(cStatus<smallest){
            smallest=cStatus;
        }
    }
    for(int index=0;index<currentPoints.size();index++){
        cv::Point2f currentPoint = currentPoints[index];
        float cStatus = errors[index];
        // std::cout << "New Value:" << currentFace_feature.x << "," << currentFace_feature.y << std::endl;
        // std::cout << "Old Value:" << next_face_features[index].x << "," << next_face_features[index].y << std::endl;
        if(highest>(smallest+0.1)*11){//Obstacle hiding face (TODO: improve) 
            boolupdateMat=false;
            std::cout << "BOM!" << std::endl;
            newPoints[index]=currentPoint;
        }
    }
    // std::cout << "Highest:" << highest << std::endl;
    // std::cout << "Smallest:" << smallest << std::endl;
    m_points=newPoints;
    if(boolupdateMat){
        m_mat = currentMat; 
    }
}

// std::vector<cv::Point2f> VideoFaceDetector::findCenters( std::vector< std::vector<cv::Point2f> > boxes){

//     std::vector<cv::Point2f> centers;

//     for(auto boxe : boxes){

//         cv::Point2f center(0,0);
//         float i = 0;

//         for(auto corner : boxe){
//             center = center + corner;
//             i=i+1.;
//         }
//         if(i>0) i = 1./i;
//         if(i==0) i = 1.;
//         center = center*i;
//         centers.push_back(center);
//     }

//     return centers;
// }

// std::vector<cv::Point2f> VideoFaceDetector::findGoodCenters( std::vector< std::vector <cv::Point2f> > boxes, std::vector<cv::Point2f> centers){

//     std::vector<cv::Point2f> goodCenters;

//     int index = 0;
//     for(auto boxe : boxes){

//         cv::Point2f center = centers[index];
//         cv::Point2f goodCenter(0,0);
//         double i = 0;

//         for(auto corner : boxe){
//             double delta = norm(center - corner);
//             double omega = 1./(delta*delta);
//             goodCenter = goodCenter + omega*corner;
//             i=i+omega;
//         }
//         if(i>0) i = 1./i;
//         if(i==0) i = 1.;
//         goodCenter = goodCenter*i;
//         goodCenters.push_back(goodCenter);

//         index++;
//     }

//     return goodCenters;
// }