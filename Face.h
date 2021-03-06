#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "Eye.h"
#include "Nose.h"
#include "Mouth.h"
using namespace cv;
using namespace std; 

class Face
{
public:
    Face(Mat faceMat);
    Face(vector<Point2f> eyePoints, Mat faceMat);
    ~Face();
    
    void                updateMat(Mat faceMat);

    Mat                 getMatPrevious();
    Mat                 getMat();
    vector<Eye>         getEyes();
    vector<Nose>        getNoses();
    vector<Mouth>       getMouths();
    vector<Point2f>     getPoints();

    Rect                getRectFromPoints();
    Rect                getRectPreviousFromPoints();
    Rect                getFullRectFromPoints();

    void                updatePointsFromRectFromPoints();
  
    void                setXOffsetFromRectFromPoints(float offset);
    void                setYOffsetFromRectFromPoints(float offset);

private:
    void                makePoints();
    void                makeRectFromPoints();

    Rect                m_rectPreviousFromPoints; 
    Rect                m_rectFromPoints;
    // Mat                 m_matPrevious;
    Mat                 m_mat;
    vector<Eye>         m_eyes;
    vector<Nose>        m_noses;
    vector<Mouth>       m_mouths;
    vector<Point2f>     m_points;

    float               m_xOffsetFromRectFromPoints;
    float               m_yOffsetFromRectFromPoints;

    Mat                 updateWidth(Mat   wider, Mat correct);
    Mat                 updateHeight(Mat higher, Mat correct);

    void                calculateOpticalFlow(Mat lastMat, Mat currentMat);

    void                findEyes(Mat faceMat);
    void                findNoses(Mat faceMat);
    void                findMouths(Mat faceMat);
};

