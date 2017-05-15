#include "PersonDetector.h"
#include "Person.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
using namespace cv;
using namespace std; 

const double PersonDetector::TICK_FREQUENCY = getTickFrequency();

PersonDetector::PersonDetector(const string cascadeFilePath,
                                     VideoCapture &videoCapture){
    setFaceCascade(cascadeFilePath);
    setVideoCapture(videoCapture);
    // this->start=false;
}

void PersonDetector::setVideoCapture(VideoCapture &videoCapture)
{
    m_videoCapture = &videoCapture;
}

VideoCapture *PersonDetector::videoCapture() const
{
    return m_videoCapture;
}

void PersonDetector::setFaceCascade(const string cascadeFilePath)
{
    if (m_faceCascade == NULL) {
        m_faceCascade = new CascadeClassifier(cascadeFilePath);
    }
    else {
        m_faceCascade->load(cascadeFilePath);
    }

    if (m_faceCascade->empty()) {
        cerr << "Error creating cascade classifier. Make sure the file" << endl
            << cascadeFilePath << " exists." << endl;
    }
}

CascadeClassifier *PersonDetector::faceCascade() const
{
    return m_faceCascade;
}

void PersonDetector::setResizedWidth(const int width)
{
    m_resizedWidth = max(width, 1);
}

int PersonDetector::resizedWidth() const
{
    return m_resizedWidth;
}

vector <Rect> PersonDetector::faces() const{
    vector <Rect> faces;
    for(auto person : persons){  
        Rect faceRect = person.getFaceRect();
        faceRect.x = (int)(faceRect.x / m_scale);
        faceRect.y = (int)(faceRect.y / m_scale);
        faceRect.width = (int)(faceRect.width / m_scale);
        faceRect.height = (int)(faceRect.height / m_scale);

        faces.push_back(faceRect);
     }
    return faces;
}

vector< vector <Point2f> >  PersonDetector::goodFeatures() const{
    vector< vector <Point2f> > currentFeatures;
    for(auto person : persons){  
        vector <Point2f> feature;
        for(auto point : person.getPoints()){
            Point2f point2;
            point2.x = (float)(point.x / (float)m_scale);
            point2.y = (float)(point.y/ (float)m_scale);
            feature.push_back(point2);
        }
        currentFeatures.push_back(feature);
    }
    return currentFeatures;
}

PersonDetector::~PersonDetector()
{
    if (m_faceCascade != NULL) {
        delete m_faceCascade;
    }
}

void PersonDetector::detectFaceAllSizes( Mat &frame)
{
    // Minimum face size is 1/5th of screen height
    // Maximum face size is 2/3rds of screen height
    Mat initFrame;
    cvtColor(frame, initFrame, CV_BGR2GRAY);
    m_faceCascade->detectMultiScale(frame, m_allFaces, 1.1, 3, 0,
                                    Size(frame.rows / 10, frame.cols / 10),
                                    Size(frame.rows , frame.cols));

    // printf("FRAME:\n");
    // printf("NumberX: %d\n", frame.rows);
    // printf("NumberY: %d\n", frame.cols);
    persons.clear();
    for(auto face_rect : m_allFaces){  
        Person person(face_rect, initFrame);
        persons.push_back(person);
    }
}

void PersonDetector::detectFacesGoodFeaturesToTrack(Mat frame){
    Mat nextFrame;
    cvtColor(frame, nextFrame, CV_BGR2GRAY);

    for(auto person : persons){  
        person.updateFaceMat(nextFrame);
    }
}

void PersonDetector::detectFacesTemplateMatching(Mat frame){
    Mat nextFrame;
    cvtColor(frame, nextFrame, CV_BGR2GRAY);

    for(auto person : persons){  
        person.templateMatching(nextFrame);
    }
}

void PersonDetector::getFrameAndDetect(Mat &frame)
{
    *m_videoCapture >> frame;

    // Downscale frame to m_resizedWidth width - keep aspect ratio
    m_scale = (double) min(m_resizedWidth, frame.cols) / frame.cols;
    Size resizedFrameSize = Size((int)(m_scale*frame.cols), (int)(m_scale*frame.rows));

    Mat resizedFrame;
    resize(frame, resizedFrame, resizedFrameSize);

    if(persons.size()==0){
        detectFaceAllSizes(resizedFrame); // Detect using cascades over whole image
    }else{
        // detectFacesGoodFeaturesToTrack(resizedFrame); 
        detectFacesTemplateMatching(resizedFrame);// Detect using template matching
    }
}

void PersonDetector::operator>>(Mat &frame)
{
    this->getFrameAndDetect(frame);
}