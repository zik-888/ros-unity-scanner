cmake_minimum_required(VERSION 2.8.3)
project(action_scan)

SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_compile_options(-std=c++11)


set(CMAKE_INCLUDE_GOSDK /home/lab45/SOFTWARE_GO_SDK/GO_SDK/Gocator/GoSdk)
set(CMAKE_INCLUDE_KAPI /home/lab45/SOFTWARE_GO_SDK/GO_SDK/Platform/kApi)
set(SOURCE_LIB action_scan.cpp)

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
    )

include_directories(
# include
  ${SOURCE_LIB}
  ${catkin_INCLUDE_DIRS}
  ${CMAKE_INCLUDE_GOSDK}
  ${CMAKE_INCLUDE_KAPI}
  ${HEADER_FILES}
  /home/lab45/catkin_ws/src/scanning_system_core/include
  /home/lab45/catkin_ws/src/scanning_system_core/include/actions_scan

  #${MY_HEADER}
)


add_library(action_scan STATIC ${SOURCE_LIB} ${HEADER_FILES})
#add_library(actions STATIC ${HEADER_FILES})

target_link_libraries(action_scan ${catkin_LIBRARIES}  ${HEADER_FILES})
#target_link_libraries(actions ${catkin_LIBRARIES} ${HEADER_FILES})

