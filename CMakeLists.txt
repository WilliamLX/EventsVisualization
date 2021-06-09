cmake_minimum_required(VERSION 3.5)
project(metavision_sdk_get_started)
set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)
find_package(MetavisionSDK COMPONENTS core driver REQUIRED)
find_package(OpenCV COMPONENTS highgui REQUIRED)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIRS}
)

set (sample metavision_sdk_get_started)
add_executable(${sample} src/${sample}.cpp src/arc_star_detector.cc)
target_link_libraries(${sample} MetavisionSDK::core MetavisionSDK::driver opencv_highgui)

set (writer write_events_to_file)
add_executable(${writer} src/${writer}.cpp src/arc_star_detector.cc)
target_link_libraries(${writer} MetavisionSDK::core MetavisionSDK::driver opencv_highgui)


set (writer2 write_events_to_file2)
add_executable(${writer2} src/${writer2}.cpp src/arc_star_detector.cc)
target_link_libraries(${writer2} MetavisionSDK::core MetavisionSDK::driver opencv_highgui)


set (detector metavision_arc_star_detector)
add_executable(${detector} src/${detector}.cpp src/arc_star_detector.cc)
target_link_libraries(${detector} MetavisionSDK::core MetavisionSDK::driver opencv_highgui)