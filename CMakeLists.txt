cmake_minimum_required(VERSION 2.8)
project( personDetector )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( personDetector main.cpp VideoFaceDetector.cpp  PersonDetector.cpp Person.cpp Face.cpp Eye.cpp Nose.cpp  Mouth.cpp )
target_link_libraries( personDetector ${OpenCV_LIBS} )

set(CMAKE_BUILD_TYPE Release)