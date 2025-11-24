#!/usr/bin/env python3
"""ROS2 launch file for WSG-50 UDP driver (script mode by default)."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch_ros.actions import Node


def generate_launch_description():
    com_mode_arg = DeclareLaunchArgument('com_mode', default_value='script')
    remap_arg = DeclareLaunchArgument('remap', default_value='false')
    force_arg = DeclareLaunchArgument('force', default_value='10')

    remap_group = GroupAction([
        Node(
            package='wsg_50_driver',
            executable='wsg_50_ip',
            name='wsg_50_driver',
            output='screen',
            parameters=[{
                'ip': '192.168.20.2',
                'port': 1500,
                'local_port': 5501,
                'protocol': 'udp',
                'com_mode': LaunchConfiguration('com_mode'),
                'rate': 50.0,
                'grasping_force': LaunchConfiguration('force')
            }],
            remappings=[('/joint_states', '/joint_states_gripper')]
        )
    ], condition=IfCondition(LaunchConfiguration('remap')))

    normal_node = Node(
        package='wsg_50_driver',
        executable='wsg_50_ip',
        name='wsg_50_driver',
        output='screen',
        parameters=[{
            'ip': '192.168.20.2',
            'port': 1500,
            'local_port': 5501,
            'protocol': 'udp',
            'com_mode': LaunchConfiguration('com_mode'),
            'rate': 50.0,
            'grasping_force': LaunchConfiguration('force')
        }],
        condition=UnlessCondition(LaunchConfiguration('remap'))
    )

    return LaunchDescription([
        com_mode_arg,
        remap_arg,
        force_arg,
        remap_group,
        normal_node
    ])
