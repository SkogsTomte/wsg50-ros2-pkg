#!/usr/bin/env python3
"""Minimal ROS2 launch for WSG-50 TCP driver without script-mode parameters."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    driver = Node(
        package='wsg_50_driver',
        executable='wsg_50_ip',
        name='wsg_50_driver',
        output='screen',
        parameters=[{
            'ip': '192.168.1.20',
            'port': 1000,
            'protocol': 'tcp'
        }]
    )
    return LaunchDescription([driver])
