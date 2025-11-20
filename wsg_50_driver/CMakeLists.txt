cmake_minimum_required(VERSION 3.8)
project(wsg_50_driver)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

## Find ament and dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(wsg_50_common REQUIRED)


# WSG_50_TCP version
set(DRIVER_SOURCES
  src/checksum.cpp include/wsg_50/checksum.h
  src/cmd.c include/wsg_50/cmd.h
  src/common.cpp include/wsg_50/common.h
  src/functions.cpp include/wsg_50/functions.h
  src/interface.cpp include/wsg_50/interface.h
  src/main.cpp
  src/msg.c include/wsg_50/msg.h
  src/serial.c include/wsg_50/serial.h
  src/tcp.c include/wsg_50/tcp.h
  src/udp.c include/wsg_50/udp.h)

# WSG_50_CAN version
set(DRIVER_SOURCES_CAN
  src/checksum.cpp
  src/common.cpp
  src/functions_can.cpp
  src/msg.c)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

include_directories(
  include
)

add_definitions(-DOSNAME_LINUX)
add_definitions(-g)

# C++14 required for ROS2
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

add_executable(wsg_50_ip src/main.cpp ${DRIVER_SOURCES})

ament_target_dependencies(wsg_50_ip
  rclcpp
  std_msgs
  std_srvs
  sensor_msgs
  wsg_50_common
)

install(TARGETS
  wsg_50_ip
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  include/
  DESTINATION include
)

ament_package()
