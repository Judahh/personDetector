#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "VideoFaceDetector.h"
#include "PersonDetector.h"


using namespace cv;
using namespace std; 

const cv::String    WINDOW_NAME("Camera video");
const cv::String    CASCADE_FILE_FACE("cascade/haarcascade_frontalface_default.xml");
const cv::String    CASCADE_FILE_EYE("cascade/eye.xml");
const cv::String    CASCADE_FILE_NOSE("cascade/nose.xml");
const cv::String    CASCADE_FILE_MOUTH("cascade/mouth.xml");

int main(int argc, char** argv)
{
	// Try opening camera
	cv::VideoCapture camera(0);
	//cv::VideoCapture camera("D:\\video.mp4");
	if (!camera.isOpened()) {
		fprintf(stderr, "Error getting camera...\n");
		exit(1);
	}

	cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO | cv::WINDOW_AUTOSIZE);

	// VideoFaceDetector detector(CASCADE_FILE_FACE, camera);
	PersonDetector detector(CASCADE_FILE_FACE, camera);
	cv::Mat frame;
	double fps = 0, time_per_frame;
	while (true){
		auto start = cv::getCPUTickCount();
		detector >> frame;
		auto end = cv::getCPUTickCount();

		time_per_frame = (end - start) / cv::getTickFrequency();
		fps = (15 * fps + (1 / time_per_frame)) / 16;

		printf("Time per frame: %3.3f\tFPS: %3.3f\n", time_per_frame, fps);

		
		for(auto face : detector.faces()){  
			cv::rectangle(frame, face, cv::Scalar(255, 255, 255));
    	}
		std::vector< std::vector <cv::Point2f> > goodFeatures = detector.goodFeatures();
		for(auto goodFeature : goodFeatures){ 
			for(auto goodFeatureA : goodFeature){   
				cv::circle(frame,goodFeatureA,5,cv::Scalar(255, 255, 255),-1,8,0);
			}
    	}

		// if (detector.isFaceFound())
		// {
		// 	cv::rectangle(frame, detector.face(), cv::Scalar(255, 255, 255));
		// 	// printf("RECT: %d, %d\n", detector.face().x, detector.face().y);
			
		// 	// cv::Vec3b mycolor(100,0,0);
		// 	int myradius=5;
		// 	std::vector< std::vector <cv::Point2f> > goodFeatures = detector.goodFeatures();
		// 	for (int i=0;i<goodFeatures.size();i++){
		// 		// printf("Number: %d\n", goodFeatures[i].size());
		// 		for (int j=0;j<goodFeatures[i].size();j++){
		// 			cv::circle(frame,goodFeatures[i][j],myradius,cv::Scalar(255, 255, 255),-1,8,0);
		// 		}
		// 	}
		// 	// cv::circle(frame, detector.facePosition(), 30, cv::Scalar(0, 255, 0));
		// }
		
		cv::imshow(WINDOW_NAME, frame);
		if (cv::waitKey(25) == 27) break;//ESC to EXIT
	}

	return 0;
}


// int main(int argc, char** argv)
// {
// 	// Load two images and allocate other structures
// 	Mat imgA = imread("image/1.png", CV_LOAD_IMAGE_GRAYSCALE);
// 	Mat imgB = imread("image/2.png", CV_LOAD_IMAGE_GRAYSCALE);
	
// 	Size img_sz = imgA.size();
// 	Mat imgC(img_sz,1);

// 	int win_size = 15;
// 	int maxCorners = 20; 
//     double qualityLevel = 0.05; 
//     double minDistance = 5.0; 
//     int blockSize = 3; 
//     double k = 0.04; 
//     std::vector<cv::Point2f> cornersA; 
// 	std::vector<cv::Point2f> cornersB; 
	
// 	cv::Point2f cornerA1;
// 	cornerA1.x=(float)10;
// 	cornerA1.y=(float)20;

// 	cv::Point2f cornerA2;
// 	cornerA2.x=(float)10;
// 	cornerA2.y=(float)10;

// 	cv::Point2f cornerA3;
// 	cornerA3.x=(float)10;
// 	cornerA3.y=(float)10;

// 	cv::Point2f cornerA4;
// 	cornerA4.x=(float)20;
// 	cornerA4.y=(float)20;

// 	cornersA.push_back(cornerA1);
// 	cornersA.push_back(cornerA2);
// 	cornersA.push_back(cornerA3);
// 	cornersA.push_back(cornerA4);


// 	// goodFeaturesToTrack( imgA,cornersA,maxCorners,qualityLevel,minDistance,cv::Mat());

// 	std::vector<uchar> features_found; 
// 	std::vector<float> feature_errors; 
    
// 	calcOpticalFlowPyrLK( imgA, imgB, cornersA, cornersB, features_found, feature_errors);

// 	// Make an image of the results

// 	for( int i=0; i < features_found.size(); i++ ){
// 			cout<<"Error is "<<feature_errors[i]<<endl;
// 			//continue;
	
// 		cout<<"Got it"<<endl;
// 		Point p0( ceil( cornersA[i].x ), ceil( cornersA[i].y ) );
// 		Point p1( ceil( cornersB[i].x ), ceil( cornersB[i].y ) );
// 		line( imgC, p0, p1, CV_RGB(255,255,255), 2 );
// 	}

// 	namedWindow( "ImageA", 0 );
// 	namedWindow( "ImageB", 0 );
// 	namedWindow( "LKpyr_OpticalFlow", 0 );

// 	imshow( "ImageA", imgA );
// 	imshow( "ImageB", imgB );
// 	imshow( "LKpyr_OpticalFlow", imgC );

// 	cvWaitKey(0);
	
// 	return 0;
// }