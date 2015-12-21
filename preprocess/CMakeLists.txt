cmake_minimum_required (VERSION 2.8)
project (SlidingWindow)
find_package (OpenCV REQUIRED)
add_executable ( SlidingWindow SlidingWindow.cpp)
target_link_libraries (SlidingWindow ${OpenCV_LIBS})

