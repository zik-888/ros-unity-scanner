cmake_minimum_required(VERSION 2.8.3)
project(kukavarproxy)

#set(CMAKE_INCLUDE_REMOTE_API /home/ros-industrial/CoppeliaSim/programming/remoteApi)
#set(CMAKE_INCLUDE_MASTER /home/ros-industrial/CoppeliaSim/programming/include)
#set(CMAKE_INCLUDE_LIB /home/ros-industrial/CoppeliaSim/programming/remoteApiBindings/lib/lib/Ubuntu18_04)

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)

set(SOURCE_LIB kukavarproxy.cpp)


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
)


add_library(kukavarproxy STATIC ${SOURCE_LIB})

target_link_libraries(kukavarproxy  Qt5::Core Qt5::Network ) #remoteApi)

#target_include_directories(manipulator  PUBLIC ${CMAKE_INCLUDE_REMOTE_API} ${CMAKE_INCLUDE_MASTER} ${CMAKE_INCLUDE_LIB})
