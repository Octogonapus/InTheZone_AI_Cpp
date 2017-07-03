cmake_minimum_required(VERSION 3.7)
project(itz_ai_cpp)

#-finline-small-functions is broken in MinGW g++ 5.3.0
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -O1")

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(GRIP_SOURCE_FILES src/grip/ConePipeline.cpp src/grip/RedMobileGoalPipeline.cpp src/grip/BlueMobileGoalPipeline.cpp)
file(GLOB NEAT_SOURCE_FILES neat/*.cpp neat/*.h)
set(SOURCE_FILES src/main.cpp src/HeapScanner.h src/guiHelper.h src/HeapScanner.cpp src/MatrixRepresentation.cpp src/MatrixRepresentation.h)
add_executable(itz_ai_cpp ${SOURCE_FILES} ${GRIP_SOURCE_FILES} ${NEAT_SOURCE_FILES})

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
