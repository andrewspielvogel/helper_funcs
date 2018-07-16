cmake_minimum_required(VERSION 2.8.3)
project(helper_funcs)

## Add support for C++11, supported in ROS Kinetic and newer
# add_definitions(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  )


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES helper_funcs
  CATKIN_DEPENDS roscpp
  )


# include boost
find_package(Boost REQUIRED)

include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

#add_definitions("-std=c++0x -Wall -Werror")
add_definitions("-std=c++0x -Wall")

add_library(${PROJECT_NAME} src/log.cpp src/time_util.cpp)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
