#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "Face.h"
using namespace cv;
using namespace std; 

class Person{
    public:
        Person(Rect faceRect,Mat faceMat);
        ~Person();
        vector<Point2f>     getPoints();
        void                updateFaceMat(Mat fullMat);
        void                templateMatching(Mat fullMat);
        cv::Rect            doubleRectSize(const cv::Rect &inputRect, const cv::Rect &frameSize) const;
        cv::Mat             getFaceTemplate(const cv::Mat &face);

        Face                getFace();
        Point2f             getPoint();
        Rect                getFaceRect();
private:
        Face                *m_face;
        Point2f             *m_point;
        Rect                *m_faceRect; 

        void                updateFace(Mat faceMat);
        void                updatePoint(Rect rect);  

        void                setFace(Face face);
        void                setPoint(Point2f point);
        void                setFaceRect(Rect rect); 

        void                newFace(Mat faceMat);
        void                newPoint(Rect rect);

        void                newFace(Face face);
        void                newPoint(Point2f point);
        void                newFaceRect(Rect rect); 
};

