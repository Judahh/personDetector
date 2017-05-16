#include "Person.h"
#include "Face.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

Person::Person(Rect faceRect, Mat fullMat){
    newFaceRect(faceRect);
    newPoint(getFaceRect());
    newFace(fullMat);
}


Person::~Person(){
    // if (m_faceRect != NULL) {
    //     delete faceRect;
    // }
    // if (m_faceRect != NULL) {
    //     delete faceRect;
    // }
    // if (m_faceRect != NULL) {
    //     delete faceRect;
    // }
}

void Person::newFaceRect(Rect faceRect){
    m_faceRect = new Rect(faceRect.x, faceRect.y, faceRect.width, faceRect.height);
}



void Person::newFace(Mat fullMat){
    // setFace(faceMat);
    Mat faceMat=fullMat(getFaceRect());
    // m_face(faceMat);
    m_face = new Face(faceMat);
}

void Person::newPoint(Rect rect){
    m_point = new Point2f((float)rect.x,(float)rect.y);
}

void Person::updateFace(Mat faceMat){
    m_face->updateMat(faceMat);
}

void Person::updatePoint(Rect rect){
    m_point->x=(float)rect.x;
    m_point->y=(float)rect.y;
}

void Person::setFace(Face face){
    Face tempFace = face;
    m_face = &tempFace;
}

void Person::setPoint(Point2f point){
    Point2f tempPoint = point;
    m_point = &tempPoint;
}

void Person::setFaceRect(Rect rect){
    m_faceRect->x=rect.x;
    m_faceRect->y=rect.y;
    m_faceRect->width=rect.width;
    m_faceRect->height=rect.height;
}


Face Person::getFace(){
    return *m_face;
    // return m_face;
}

Point2f Person::getPoint(){
    return *m_point;
    // return m_point;
}

Rect Person::getFaceRect(){
    return *m_faceRect;
    // return m_faceRect;
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

Rect Person::getRectFromPoints(){
    Rect tempRect = m_face->getRectFromPoints();
    cout << "NX:" << m_faceRect->x << endl;
    cout << "NY:" << m_faceRect->y << endl;
    cout << "Nwidth:" << m_faceRect->width << endl;
    cout << "Nheight:" << m_faceRect->height << endl;
    cout << "BX:" << tempRect.x << endl;
    cout << "BY:" << tempRect.y << endl;
    cout << "Bwidth:" << tempRect.width << endl;
    cout << "Bheight:" << tempRect.height << endl;
    tempRect.x = tempRect.x + m_faceRect->x;
    tempRect.y = tempRect.y + m_faceRect->y;
    cout << "X:" << tempRect.x << endl;
    cout << "Y:" << tempRect.y << endl;
    return tempRect;
}

void Person::updateFaceMat(Mat fullMat){
    setFaceRect(getRectFromPoints());
    cout << "AX:" << m_faceRect->x << endl;
    cout << "AY:" << m_faceRect->y << endl;
    Mat faceMat=fullMat(getFaceRect());
    cout << "P!" << endl;
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
    
    setFaceRect(faceRect);

    Mat faceMat=fullMat(getFaceRect());
    updatePoint(faceRect);
    updateFace(faceMat);  

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