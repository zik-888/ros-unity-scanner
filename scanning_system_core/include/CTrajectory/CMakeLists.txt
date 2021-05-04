cmake_minimum_required(VERSION 2.8.3)
project(CTrajectory)

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)


set(SOURCE_LIB CTrajectory.cpp)

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
    /home/lab45/catkin_ws/src/scanning_system_core/include
    /home/lab45/catkin_ws/src/scanning_system_core/include/CTrajectory
    )

include_directories(
# include
  ${SOURCE_LIB}
  ${catkin_INCLUDE_DIRS}
  ${CMAKE_INCLUDE_GOSDK}
  ${CMAKE_INCLUDE_KAPI}
  ${HEADER_FILES}
  /home/lab45/catkin_ws/src/scanning_system_core/include
  /home/lab45/catkin_ws/src/scanning_system_core/include/CTrajectory

  #${MY_HEADER}
)


add_library(CTrajectory STATIC ${SOURCE_LIB} ${HEADER_FILES})
#add_library(actions STATIC ${HEADER_FILES})

target_link_libraries(CTrajectory ${catkin_LIBRARIES}  ${HEADER_FILES})
#target_link_libraries(actions ${catkin_LIBRARIES} ${HEADER_FILES})

