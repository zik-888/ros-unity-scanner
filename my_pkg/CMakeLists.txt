cmake_minimum_required(VERSION 2.8.3)
project(my_pkg)

## Compile as C++11, supported in ROS Kinetic and newer

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  #fake_ar_publisher
  message_generation
  std_msgs
  geometry_msgs
  visualization_msgs

)

find_package(Qt5Core)
find_package(Qt5Network)


################################################
## Declare ROS messages, services and actions ##
################################################



## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Arturmsg.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  TrajectoryService.srv
  ScanPositionService.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
   geometry_msgs
)


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES my_pkg
#  CATKIN_DEPENDS roscpp
CATKIN_DEPENDS roscpp std_msgs visualization_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  include/manipulator
  ${catkin_INCLUDE_DIRS}
  #${CMAKE_INCLUDE_REMOTE_API}
  #${CMAKE_INCLUDE_MASTER}
  #${CMAKE_INCLUDE_LIB}
)



#add_dependencies(${SOURCE_LIB} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/my_pkg.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/my_pkg_node.cpp)
#add_executable(ros_test_node src/test_node.cpp include/kukavarproxy_interface.cpp include/eki_interface.cpp )

#add_executable(ros_test_node src/test_node.cpp )
add_executable(ros_com_test_node src/srvtest.cpp )
#add_executable(ros_vrep_scan_man_node src/vrep_scan_man.cpp )
#add_executable(ros_vrep_work_man_node src/vrep_work_man.cpp )
add_executable(ros_kuka_node src/kuka_node.cpp )
add_executable(ros_kuka_srv_node src/kuka_srv_node.cpp )
add_executable(ros_tcp_node src/tcp_node.cpp )
add_executable(ros_mitsubishi_node src/mitsubishi_node.cpp )

add_subdirectory(include/manipulator)

add_subdirectory(include/kukavarproxy)

add_subdirectory(include/eki_interface)

add_subdirectory(include/mitsubishi)

#add_dependencies(ros_test_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(ros_com_test_node   ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(ros_vrep_scan_man_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(ros_vrep_work_man_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(ros_kuka_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(ros_kuka_srv_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(ros_tcp_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(ros_mitsubishi_node  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#target_link_libraries(ros_test_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network remoteApi manipulator)
target_link_libraries(ros_com_test_node  ${catkin_LIBRARIES} Qt5::Core Qt5::Network manipulator) #remoteApi
#target_link_libraries(ros_vrep_scan_man_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network manipulator)
#target_link_libraries(ros_vrep_work_man_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network remoteApi manipulator)
target_link_libraries(ros_kuka_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network manipulator kukavarproxy eki_interface) # remoteApi
target_link_libraries(ros_kuka_srv_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network manipulator kukavarproxy eki_interface) # remoteApi
target_link_libraries(ros_tcp_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network manipulator) # remoteApi
target_link_libraries(ros_mitsubishi_node ${catkin_LIBRARIES} Qt5::Core Qt5::Network manipulator mitsubishi) #remoteApi
