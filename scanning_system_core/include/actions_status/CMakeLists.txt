cmake_minimum_required(VERSION 2.8.3)
project(action_status)

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)

set(SOURCE_LIB action_status.cpp)
set(HEADER_FILES
    /home/lab45/catkin_ws/src/scanning_system_core/include
    )

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  std_msgs
  geometry_msgs
  visualization_msgs
  Qt5Core
  Qt5Network
)


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${SOURCE_LIB}
  ${HEADER_FILES}
)

add_library(action_status STATIC ${SOURCE_LIB} ${HEADER_FILES})

target_link_libraries(action_status ${catkin_LIBRARIES} ${SOURCE_LIB} ${HEADER_FILES})

