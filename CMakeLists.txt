cmake_minimum_required(VERSION 2.8.3)
project(dialogue_as)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  actionlib_msgs
  actionlib
  roscpp
  rospy
  std_msgs
)

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate actions in the 'action' folder
add_action_files(
  DIRECTORY action
  FILES
  dialogue_action.action
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  actionlib_msgs
  std_msgs
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
# INCLUDE_DIRS include
#  LIBRARIES dialogue_as
 CATKIN_DEPENDS actionlib_msgs roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(dialogue_as src/main.cpp)
add_dependencies(dialogue_as dialogue_as_gencpp)
target_link_libraries(dialogue_as ${catkin_LIBRARIES})

add_executable(dialogue_as_tester_yes_no src/test_yes_no.cpp)
add_dependencies(dialogue_as_tester_yes_no dialogue_as_gencpp)
target_link_libraries(dialogue_as_tester_yes_no ${catkin_LIBRARIES})

add_executable(dialogue_as_tester_verb_subject src/test_verb_subject.cpp)
add_dependencies(dialogue_as_tester_verb_subject dialogue_as_gencpp)
target_link_libraries(dialogue_as_tester_verb_subject ${catkin_LIBRARIES})
