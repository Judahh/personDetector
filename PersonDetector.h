#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "Person.h"
using namespace cv;
using namespace std; 

class PersonDetector
{
public:
    PersonDetector(const string cascadeFilePath,
                      VideoCapture &videoCapture);
    ~PersonDetector();

    void                    getFrameAndDetect(Mat &frame);
    void                    operator>>(Mat &frame);
    void                    setVideoCapture(VideoCapture &videoCapture);
    VideoCapture*       videoCapture() const;
    void                    setFaceCascade(const string cascadeFilePath);
    CascadeClassifier*  faceCascade() const;
    void                    setResizedWidth(const int width);
    int                     resizedWidth() const;
    vector<Rect>                faces() const;
    Point               facePosition() const;
    vector< vector <Point2f> >  goodFeatures() const;

private:
    static const double     TICK_FREQUENCY;

    VideoCapture*       m_videoCapture = NULL;
    CascadeClassifier*  m_faceCascade = NULL;
    vector<Rect>        m_allFaces;
    vector<Person>      persons;

    double                  m_scale;
    int                     m_resizedWidth = 320;

    vector<vector <Point2f> > faces_features;
    
    void        detectFaceAllSizes( Mat &frame);
    void        detectFacesGoodFeaturesToTrack(Mat frame);
    void        detectFacesTemplateMatching(Mat frame);
};

