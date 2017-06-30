cmake_minimum_required(VERSION 3.7)
project(itz_ai_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)# COMPONENTS core videoio imgproc highgui)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp ConePipeline.cpp)
add_executable(itz_ai_cpp ${SOURCE_FILES})

target_link_libraries(itz_ai_cpp ${OpenCV_LIBS})
