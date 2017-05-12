#include "Person.h"
#include "Face.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Person::Person(Rect faceRect, Mat fullMat){
    m_faceRect = faceRect;
    Mat faceMat=fullMat(faceRect);
   
    m_point = new Point2f((float)faceRect.x,(float)faceRect.y);
    m_face = new Face(faceMat);
}


Person::~Person(){
    // if (m_faceCascade != NULL) {
    //     delete m_faceCascade;
    // }
}

Face Person::getFace(){
    return *m_face;
}

Point2f Person::getPoint(){
    return *m_point;
}

Rect Person::getFaceRect(){
    return m_faceRect;
}

vector<Point2f> Person::getPoints(){
    vector<Point2f> points;
    for(auto point : m_face->getPoints()){
        point.x=point.x+m_point->x;
        point.y=point.y+m_point->y;
        points.push_back(point);
    }
    return points;
}

void Person::updateFaceMat(Mat fullMat){
    Mat faceMat=fullMat(m_faceRect);
    m_face->updateMat(faceMat);
}

cv::Mat Person::getFaceTemplate(const cv::Mat &face){
    Rect faceRect = Rect(face.cols/4, face.rows/4, face.cols/2, face.rows/2);

    cv::Mat faceTemplate = face(faceRect).clone();
    return faceTemplate;
}

void Person::templateMatching(Mat fullMat){
    // "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED"
    int matchMethod=CV_TM_SQDIFF_NORMED;// CV_TM_CCOEFF;
    Mat result;
    imshow("Initial", m_face->getMat());
    Mat faceTemplate = getFaceTemplate(m_face->getMat());
    matchTemplate( fullMat, faceTemplate, result, matchMethod );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;

    Point matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    if( matchMethod  == CV_TM_SQDIFF || matchMethod == CV_TM_SQDIFF_NORMED ){ matchLoc = minLoc; }
    else{ matchLoc = maxLoc; }

    Rect faceRect = Rect(minLoc.x, minLoc.y, faceTemplate.cols, faceTemplate.rows);
    faceRect = doubleRectSize(faceRect, Rect(0, 0, fullMat.cols, fullMat.rows));

    imshow("Template", faceTemplate);
    

    m_faceRect = faceRect;

    Mat faceMat=fullMat(faceRect);
    m_point->x = (float)faceRect.x;
    m_point->y = (float)faceRect.y;
    m_face = new Face(faceMat);

    imshow("FOUND", faceMat);
    imshow("Replaced", m_face->getMat());

    // cv::rectangle(frame, faceRect, cv::Scalar(0, 255, 0));
}

cv::Rect Person::doubleRectSize(const cv::Rect &inputRect, const cv::Rect &frameSize) const{
    cv::Rect outputRect;
    // Double rect size
    outputRect.width = inputRect.width * 2;
    outputRect.height = inputRect.height * 2;

    // Center rect around original center
    outputRect.x = inputRect.x - inputRect.width / 2;
    outputRect.y = inputRect.y - inputRect.height / 2;

    // Handle edge cases
    if (outputRect.x < frameSize.x) {
        outputRect.width += outputRect.x;
        outputRect.x = frameSize.x;
    }
    if (outputRect.y < frameSize.y) {
        outputRect.height += outputRect.y;
        outputRect.y = frameSize.y;
    }

    if (outputRect.x + outputRect.width > frameSize.width) {
        outputRect.width = frameSize.width - outputRect.x;
    }
    if (outputRect.y + outputRect.height > frameSize.height) {
        outputRect.height = frameSize.height - outputRect.y;
    }

    return outputRect;
}