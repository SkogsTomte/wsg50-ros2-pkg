#!/usr/bin/env python3
"""ROS2 launch: keyboard teleoperation for simulated WSG-50."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    teleop = Node(
        package='wsg_50_simulation',
        executable='wsg_50_sim_keyboard_teleop',
        name='wsg_50_sim_keyboard_teleop',
        output='screen'
    )
    return LaunchDescription([teleop])
