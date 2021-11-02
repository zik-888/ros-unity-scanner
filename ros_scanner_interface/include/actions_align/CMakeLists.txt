cmake_minimum_required(VERSION 2.8.3)
project(action_align)

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)

set(SOURCE_LIB action_align.cpp)
set(HEADER_FILES
    /home/lab45/catkin_ws/src/ros_scanner_interface/include
    )

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  std_msgs
  geometry_msgs
  visualization_msgs
)


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${SOURCE_LIB}
  ${HEADER_FILES}
)

add_library(action_align STATIC ${SOURCE_LIB} ${HEADER_FILES})

target_link_libraries(action_align ${catkin_LIBRARIES} ${SOURCE_LIB} ${HEADER_FILES})

