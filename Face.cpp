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
    findEyes(faceMat);
    findNoses(faceMat);
    findMouths(faceMat);
    makePoints();
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

void Face::findEyes(Mat faceMat){
    CascadeClassifier eyeCascade("cascade/eye.xml");
    vector<cv::Rect>   allEyes;
    eyeCascade.detectMultiScale(faceMat, allEyes, 1.1, 3, 0,
    Size(faceMat.rows / 50, faceMat.cols / 50),
    Size(faceMat.rows, faceMat.cols));

    // cout << "Eyes:" << allEyes.size() << endl;
    for(auto eyeRect : allEyes){
        Eye eye(eyeRect);
        m_eyes.push_back(eye);
    }
}

void Face::findNoses(Mat faceMat){
    CascadeClassifier noseCascade("cascade/nose.xml");
    vector<cv::Rect>   allNoses;
    noseCascade.detectMultiScale(faceMat, allNoses, 1.1, 3, 0,
    Size(faceMat.rows / 50, faceMat.cols / 50),
    Size(faceMat.rows, faceMat.cols));

    // cout << "Noses:" << allNoses.size() << endl;
    for(auto noseRect : allNoses){
        Nose nose(noseRect);
        m_noses.push_back(nose);
    }
}

void Face::findMouths(Mat faceMat){
    CascadeClassifier mouthCascade("cascade/mouth.xml");
    vector<cv::Rect>   allMouths;
    mouthCascade.detectMultiScale(faceMat, allMouths, 1.1, 3, 0,
    Size(faceMat.rows / 50, faceMat.cols / 50),
    Size(faceMat.rows, faceMat.cols));

    // cout << "Mouths:" << allMouths.size() << endl;
    for(auto mouthRect : allMouths){
        Mouth mouth(mouthRect);
        m_mouths.push_back(mouth);
    }
}

// Mat Face::getMatPrevious(){
//     return m_matPrevious;
// }

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

void Face::makePoints(){
    vector<Point2f> points;
    // cout << "E:" << m_eyes.size() << endl;
    // cout << "N:" << m_noses.size() << endl;
    // cout << "M:" << m_mouths.size() << endl;

    for(auto eye : m_eyes){
        for(auto point : eye.getPoints()){
            points.push_back(point);
        }
    }   
    
    for(auto nose : m_noses){
        for(auto point : nose.getPoints()){
            points.push_back(point);
        }
    }  

    for(auto mouth : m_mouths){
        for(auto point : mouth.getPoints()){
            points.push_back(point);
        }
    }  

    // cout << "P:" << points.size() << endl;
    // for(auto point : points){
    //     cout << "PX:" << point.x << endl;
    //     cout << "PY:" << point.x << endl;
    // }
    m_points = points;
    makeRectFromPoints();
}

Mat Face::updateWidth(Mat   wider, Mat correct){
    Rect tempRect((wider.cols - correct.cols)/2, 0, correct.cols, wider.rows);
    return wider(tempRect);
}

Mat Face::updateHeight(Mat higher, Mat correct){
    Rect tempRect(0, (higher.rows - correct.rows)/2, higher.cols, correct.rows);
    return higher(tempRect);
}

void Face::calculateOpticalFlow(Mat lastMat, Mat currentMat){
    vector<uchar> status;
    vector<float> errors;
    vector<Point2f> currentPoints = m_points;
    // std::cout << "POINTS:" << currentPoints.size() << std::endl; 
    vector<Point2f> newPoints;

    bool boolupdateMat=true;
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

        // if(highest>(smallest+0.1)*11){//Obstacle hiding face (TODO: improve) 
        //     boolupdateMat=false;
        //     std::cout << "BOM!" << std::endl;
        //     newPoints[index]=currentPoint;
        // }
    }

    for(int index=0;index<newPoints.size();index++){
        if(newPoints[index].x<=0 || 
           newPoints[index].y<=0 ||
           newPoints[index].x>=m_mat.rows ||
           newPoints[index].y>=m_mat.cols){
            // cout << "BOM!" << newPoints.size() << endl;
            newPoints.erase(newPoints.begin()+index);
            index--;
            // cout << "N:" << newPoints.size() << endl;
        }
    }
    // std::cout << "Highest:" << highest << std::endl;
    // std::cout << "Smallest:" << smallest << std::endl;
    m_points=newPoints;
    if(boolupdateMat){
        // m_matPrevious = m_mat; 
        m_mat = currentMat;
    }
}

Rect Face::getRectFromPoints(){
    return m_rectFromPoints;
}

Rect Face::getRectPreviousFromPoints(){
    return m_rectPreviousFromPoints;
}

void Face::makeRectFromPoints(){
    float smallestX=1000000;
    float smallestY=1000000;
    float biggestX=0;
    float biggestY=0;
    for(auto point : m_points){
        if(point.x>biggestX){
            biggestX=point.x;
        }
        if(point.x<smallestX && point.x>=0){
            smallestX=point.x;
        }
        if(point.y>biggestY){
            biggestY=point.y;
        }
        if(point.y<smallestY && point.y>=0){
            smallestY=point.y;
        }
    }
    m_rectPreviousFromPoints = m_rectFromPoints;
    m_rectFromPoints = Rect(smallestX, smallestY, biggestX-smallestX, biggestY-smallestY);
}

Rect Face::getFullRectFromPoints(){
    Rect currentRect = getRectFromPoints();
    Rect previousRect = getRectPreviousFromPoints();
    if(previousRect.width==0||previousRect.height==0){
        previousRect=currentRect;
        m_rectPreviousFromPoints = m_rectFromPoints;
    }
    Mat previousMat = getMat();

    // cout << "previous x:" << previousRect.x << endl;
    // cout << "previous y:" << previousRect.y << endl;
    // cout << "previous width:" << previousRect.width << endl;
    // cout << "previous height:" << previousRect.height << endl;

    float widthOffset = previousMat.cols - previousRect.width;
    float heightOffset = previousMat.rows - previousRect.height;
    float xOffset = 0 - previousRect.x;
    float yOffset = 0 - previousRect.y;

    cout << "widthOffset:" << widthOffset << endl;
    cout << "heightOffset:" << heightOffset << endl;
    cout << "xOffset:" << xOffset << endl;
    cout << "yOffset:" << yOffset << endl;

    float newX = currentRect.x+xOffset;
    float newY = currentRect.y+yOffset;
    float newWidth = currentRect.width+widthOffset;
    float newHeight = currentRect.height+heightOffset;

    cout << "NEW x:" << newX << endl;
    cout << "NEW y:" << newY << endl;
    cout << "NEW width:" << newWidth << endl;
    cout << "NEW height:" << newHeight << endl;
    
    return Rect(newX, newY, newWidth, newHeight);
}

void Face::updateMat(Mat currentMat){
    Mat lastMat = m_mat;
    
    if((lastMat.cols!=currentMat.cols)||(lastMat.rows!=currentMat.rows)){
        // std::cout << "LAST:" << std::endl; 
        // std::cout << "COLS:" << lastMat.cols << std::endl; 
        // std::cout << "ROWS:" << lastMat.rows << std::endl; 
        // std::cout << "NEW:" << std::endl; 
        // std::cout << "COLS:" << currentMat.cols << std::endl; 
        // std::cout << "ROWS:" << currentMat.rows << std::endl; 

        if(lastMat.cols!=currentMat.cols){
            if(lastMat.cols>currentMat.cols){
                lastMat=updateWidth(lastMat,currentMat);
            }
            if(lastMat.cols<currentMat.cols){
                currentMat=updateWidth(currentMat,lastMat);
            }
        }

        if(lastMat.rows!=currentMat.rows){
            if(lastMat.rows>currentMat.rows){
                lastMat=updateHeight(lastMat,currentMat);
            }

            if(lastMat.rows<currentMat.rows){
                currentMat=updateHeight(currentMat,lastMat);
            }
        }

        // std::cout << "LAST:" << std::endl; 
        // std::cout << "COLS:" << lastMat.cols << std::endl; 
        // std::cout << "ROWS:" << lastMat.rows << std::endl; 
        // std::cout << "NEW:" << std::endl; 
        // std::cout << "COLS:" << currentMat.cols << std::endl; 
        // std::cout << "ROWS:" << currentMat.rows << std::endl; 
    }

    vector<Point2f> currentPoints = m_points;
    // std::cout << "POINTS:" << currentPoints.size() << std::endl; 

    if(currentPoints.size()>0){
        calculateOpticalFlow(lastMat, currentMat);
    }else{
        findEyes(lastMat);
        findNoses(lastMat);
        findMouths(lastMat);
        makePoints();
        if(currentPoints.size()>0){
            calculateOpticalFlow(lastMat, currentMat);
        }
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