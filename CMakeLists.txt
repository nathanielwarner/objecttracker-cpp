cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(ObjectTracker)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS date_time REQUIRED)
add_executable(ObjectTracker window.cpp coloredobject.cpp)
target_link_libraries(ObjectTracker ${OpenCV_LIBS} ${Boost_LIBRARIES})
set_property(TARGET ObjectTracker PROPERTY CXX_STANDARD 11)
set_property(TARGET ObjectTracker PROPERTY CXX_STANDARD_REQUIRED ON)
