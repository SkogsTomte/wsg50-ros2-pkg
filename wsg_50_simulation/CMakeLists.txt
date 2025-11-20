cmake_minimum_required(VERSION 3.8)
project(wsg_50_simulation)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(urdf REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(wsg_50_common REQUIRED)

# wsg_50_sim_keyboard_teleop executable
add_executable(wsg_50_sim_keyboard_teleop src/wsg_50_keyboard_teleop.cpp)
ament_target_dependencies(wsg_50_sim_keyboard_teleop
  rclcpp
  std_msgs
  std_srvs
  urdf
  wsg_50_common
)

# wsg_50_sim_driver executable
add_executable(wsg_50_sim_driver src/wsg_50_sim_driver.cpp)
ament_target_dependencies(wsg_50_sim_driver
  rclcpp
  std_msgs
  std_srvs
  urdf
  sensor_msgs
  wsg_50_common
)

# Install executables
install(TARGETS
  wsg_50_sim_keyboard_teleop
  wsg_50_sim_driver
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
