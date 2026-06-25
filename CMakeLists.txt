cmake_minimum_required(VERSION 3.8)
project(lidar_filter_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Bağımlılıklar
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENT filters)

# PCL için kritik eklemeler
include_directories(${PCL_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})
link_directories(${PCL_LIBRARY_DIRS})

# Executable
add_executable(lidar_filter_node src/lidar_filter_node.cpp)

# Linkleme
target_link_libraries(lidar_filter_node
  ${PCL_LIBRARIES}
)

# ROS 2 bağımlılıklarını target'a bağlıyoruz
ament_target_dependencies(lidar_filter_node
  rclcpp
  sensor_msgs
  pcl_conversions
)

# Kurulum (Install) ayarları
install(TARGETS
  lidar_filter_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
