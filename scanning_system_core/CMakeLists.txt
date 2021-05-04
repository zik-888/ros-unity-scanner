cmake_minimum_required(VERSION 2.8.3)
project(scanning_system_core)

set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

##SET(CMAKE_INCLUDE_REMOTE_API /home/ros-industrial/CoppeliaSim_Edu_V4_0_0_Ubuntu18_04/programming/remoteApi)
##SET(CMAKE_INCLUDE_MASTER /home/ros-industrial/include-master)
SET(CMAKE_CXX_FLAGS -pthread)


## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)
add_definitions(-DQT_NO_VERSION_TAGGING)

find_package( catkin REQUIRED COMPONENTS
  pcl_ros
  roscpp
  message_generation
  geometry_msgs
  shape_msgs
  std_msgs
  actionlib
  actionlib_msgs
)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Qt5Core)
find_package(Qt5Network)


add_message_files(
  FILES
  Mesh.msg
  Status.msg
  StartScanMsg.msg
  TrajectoryMsg.msg
  StatusMsg.msg
  StartMsg.msg
  PoseRobotMsg.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  AddTwoInts.srv
  PolygonModel.srv
  TrajectoryService.srv
  StatusService.srv
  StartService.srv
  PoseRobotService.srv
  ScanPositionService.srv
)

add_action_files(
  DIRECTORY action
  FILES Scanning.action
        Processing.action
)


generate_messages(
  DEPENDENCIES
  geometry_msgs
  shape_msgs
  std_msgs
  actionlib_msgs  # Or other packages containing msgs
)



catkin_package(
 #  INCLUDE_DIRS include
 #  LIBRARIES myworkcell_core
   CATKIN_DEPENDS 
     roscpp 
     message_runtime
     geometry_msgs
     std_msgs
     actionlib_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide


set(CMAKE_INCLUDE_GOSDK /home/lab45/SOFTWARE_GO_SDK/GO_SDK/Gocator/GoSdk)
set(CMAKE_INCLUDE_KAPI /home/lab45/SOFTWARE_GO_SDK/GO_SDK/Platform/kApi)
##set(MY_HEADER /home/ros-industrial/catkin_ws/src/scanning_system_core/src)
set(HEADER_FILES
    ${CMAKE_INCLUDE_GOSDK}/GoSdk
    ${CMAKE_INCLUDE_KAPI}/kApi
    /home/lab45/catkin_ws/src/scanning_system_core/include
    /home/lab45/catkin_ws/src/scanning_system_core/include/actions_scan
    /home/lab45/catkin_ws/src/scanning_system_core/include/CScanner
    /home/lab45/catkin_ws/src/scanning_system_core/include/CTrajectory
    /home/lab45/catkin_ws/src/scanning_system_core/include/CPolyModel)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${CMAKE_INCLUDE_GOSDK}
  ${CMAKE_INCLUDE_KAPI}
  ${HEADER_FILES}
  #${Qt5Core_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  include
  include/actions_scan
  include/CScanner
  include/CTrajectory
  include/CPolyModel

  #${MY_HEADER}
)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
## Specify additional locations of header files
## Your package locations should be listed before other locations

add_executable(scanning_node src/scanning_node.cpp)# ${HEADER_FILES} ${MY_HEADER} include/actions_scan include)
add_executable(add_two_ints_client src/add_two_ints_client.cpp)

## add_executable(gomodes src/gomodes.cpp ${HEADER_FILES} ${MY_HEADER})

##add_subdirectory(include)
add_subdirectory(include/actions_scan)
add_subdirectory(include/CScanner)
add_subdirectory(include/CTrajectory)
add_subdirectory(include/CPolyModel)

add_dependencies(scanning_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(add_two_ints_client ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_include_directories(
    scanning_node PRIVATE
    ${HEADER_FILES}
    ${CMAKE_INCLUDE_GOSDK}
    ${CMAKE_INCLUDE_KAPI}
    ${CMAKE_INCLUDE_REMOTE_API}
    ${CMAKE_INCLUDE_MASTER})
## target_include_directories(gomodes PRIVATE ${HEADER_FILES} ${CMAKE_INCLUDE_GOSDK} ${CMAKE_INCLUDE_KAPI} ${MY_HEADER})


target_link_libraries(scanning_node ${catkin_LIBRARIES} ${PCL_LIBRARIES})
target_link_libraries(add_two_ints_client ${catkin_LIBRARIES})

target_link_libraries(scanning_node /home/lab45/SOFTWARE_GO_SDK/GO_SDK/lib/linux_x64d/libGoSdk.so)
target_link_libraries(scanning_node /home/lab45/SOFTWARE_GO_SDK/GO_SDK/lib/linux_x64d/libkApi.so)
target_link_libraries(scanning_node CScanner CTrajectory CPolyModel action_scan)
##target_include_directories(scanning_node PRIVATE ${CMAKE_INCLUDE_REMOTE_API})
##target_include_directories(scanning_node PRIVATE ${CMAKE_INCLUDE_MASTER})

target_include_directories(scanning_node PRIVATE ${CMAKE_INCLUDE_REMOTE_API})
target_include_directories(scanning_node PRIVATE ${CMAKE_INCLUDE_MASTER})
target_include_directories(scanning_node PRIVATE ${HEADER_FILES})


