cmake_minimum_required(VERSION 3.7)
project(itz_ai_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp ConePipeline.cpp)
add_executable(itz_ai_cpp ${SOURCE_FILES})

set(VJOY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/vJoy_SDK/inc)
set(VJOY_LIBRARY_DIRECTORIES ${PROJECT_SOURCE_DIR}/vJoy_SDK/lib)
set(VJOY_LIBRARIES ${VJOY_LIBRARY_DIRECTORIES}/vJoyInterface.lib)
set(VJOY_DLLS ${VJOY_LIBRARY_DIRECTORIES}/vJoyInterface.dll)

target_include_directories(itz_ai_cpp PUBLIC ${VJOY_INCLUDE_DIRECTORIES})
target_link_libraries(itz_ai_cpp PUBLIC ${OpenCV_LIBS} ${VJOY_LIBRARIES})

get_property(dll TARGET itz_ai_cpp PROPERTY IMPORTED_LOCATION_DEBUG)
file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR})
get_property(dll TARGET itz_ai_cpp PROPERTY IMPORTED_LOCATION_RELEASE)
file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${VJOY_DLLS} DESTINATION ${PROJECT_BINARY_DIR})