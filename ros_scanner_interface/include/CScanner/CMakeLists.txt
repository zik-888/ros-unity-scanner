cmake_minimum_required(VERSION 2.8.3)
project(CScanner)

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)


set(SOURCE_LIB CScanner.cpp)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  std_msgs
  geometry_msgs
  visualization_msgs
)

set(HEADER_FILES
    ${CMAKE_INCLUDE_GOSDK}/GoSdk
    ${CMAKE_INCLUDE_KAPI}/kApi
    /home/lab45/catkin_ws/src/ros_scanner_interface/include
#    /home/ros-industrial/catkin_ws/src/ros_scanner_interface/include/CScanner
    )

include_directories(
# include
  ${SOURCE_LIB}
  ${catkin_INCLUDE_DIRS}
  ${CMAKE_INCLUDE_GOSDK}
  ${CMAKE_INCLUDE_KAPI}
  ${HEADER_FILES}
  /home/lab45/catkin_ws/src/ros_scanner_interface/include
#  /home/ros-industrial/catkin_ws/src/ros_scanner_interface/include/CScanner

  #${MY_HEADER}
)


add_library(CScanner ${SOURCE_LIB} ${HEADER_FILES})
#add_library(actions STATIC ${HEADER_FILES})

target_link_libraries(CScanner ${catkin_LIBRARIES}  ${HEADER_FILES})
#target_link_libraries(actions ${catkin_LIBRARIES} ${HEADER_FILES})

