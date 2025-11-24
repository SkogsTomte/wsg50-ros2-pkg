#!/usr/bin/env python3
"""ROS2 launch for simulated WSG-50 gripper.

Loads URDF, starts robot_state_publisher and spawns entity into Gazebo (if gazebo_ros available).
Controller spawners from ROS1 are omitted pending ros2_control port.
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time', default_value='true', description='Use simulation time'
    )

    pkg_share = get_package_share_directory('wsg_50_simulation')
    urdf_file = os.path.join(pkg_share, 'urdf', 'wsg_50.urdf')

    # Robot State Publisher
    robot_description = ''
    try:
        with open(urdf_file, 'r') as f:
            robot_description = f.read()
    except Exception:
        pass

    rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        parameters=[{
            'use_sim_time': LaunchConfiguration('use_sim_time'),
            'robot_description': robot_description
        }]
    )

    # Spawn gripper into Gazebo (optional; will fail gracefully if gazebo_ros not present)
    spawn = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        name='spawn_gripper',
        arguments=['-entity', 'wsg_50', '-file', urdf_file],
        output='screen'
    )

    sim_driver = Node(
        package='wsg_50_simulation',
        executable='wsg_50_sim_driver',
        name='wsg_50',
        output='screen',
        parameters=[{
            'vel_pub_l_Topic': '/wsg_50_gl/command',
            'vel_pub_r_Topic': '/wsg_50_gr/command'
        }]
    )

    return LaunchDescription([
        use_sim_time_arg,
        rsp,
        spawn,
        sim_driver
    ])
