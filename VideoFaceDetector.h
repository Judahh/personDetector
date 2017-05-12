#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "Person.h"

class VideoFaceDetector
{
public:
    VideoFaceDetector(const std::string cascadeFilePath,
                      cv::VideoCapture &videoCapture);
    ~VideoFaceDetector();

    cv::Point               getFrameAndDetect(cv::Mat &frame);
    cv::Point               operator>>(cv::Mat &frame);
    void                    setVideoCapture(cv::VideoCapture &videoCapture);
    cv::VideoCapture*       videoCapture() const;
    void                    setFaceCascade(const std::string cascadeFilePath);
    cv::CascadeClassifier*  faceCascade() const;
    void                    setResizedWidth(const int width);
    int                     resizedWidth() const;
	bool					isFaceFound() const;
    cv::Rect                face() const;
    cv::Point               facePosition() const;
    std::vector< std::vector <cv::Point2f> >  goodFeatures() const;
    void                    setTemplateMatchingMaxDuration(const double s);
    double                  templateMatchingMaxDuration() const;
    void                    detectFacesGoodFeaturesToTrack(cv::Mat frame);

private:
    static const double     TICK_FREQUENCY;

    cv::VideoCapture*       m_videoCapture = NULL;
    cv::CascadeClassifier*  m_faceCascade = NULL;
    std::vector<cv::Rect>   m_allFaces;
    std::vector<Person>     m_persons;
    cv::Rect                m_trackedFace;
    cv::Rect                m_faceRoi;
    cv::Mat                 m_faceTemplate;
    cv::Mat                 m_matchingResult;
    bool                    m_templateMatchingRunning = false;
    int64                   m_templateMatchingStartTime = 0;
    int64                   m_templateMatchingCurrentTime = 0;
    bool                    m_foundFace = false;
    double                  m_scale;
    int                     m_resizedWidth = 320;
    cv::Point               m_facePosition;
    double                  m_templateMatchingMaxDuration = 3;

    cv::Mat     prev_frame;
    // cv::CascadeClassifier face_cascade;
    // int         update_rate;
    // bool         start;
    std::vector<cv::Point2f> faces_centers;
    std::vector<std::vector <cv::Point2f> > faces_features;
    

    cv::Rect    doubleRectSize(const cv::Rect &inputRect, const cv::Rect &frameSize) const;
    cv::Rect    biggestFace(std::vector<cv::Rect> &faces) const;
    cv::Point   centerOfRect(const cv::Rect &rect) const;
    cv::Mat     getFaceTemplate(const cv::Mat &frame, cv::Rect face);
    void        detectFaceAllSizes( cv::Mat &frame);
    void        detectFaceAroundRoi(const cv::Mat &frame);
    void        detectFacesTemplateMatching(const cv::Mat &frame);

    
};

