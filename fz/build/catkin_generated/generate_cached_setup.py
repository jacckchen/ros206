# -*- coding: utf-8 -*-
from __future__ import print_function

import os
import stat
import sys

# find the import for catkin's python package - either from source space or from an installed underlay
if os.path.exists(os.path.join('/opt/ros/noetic/share/catkin/cmake', 'catkinConfig.cmake.in')):
    sys.path.insert(0, os.path.join('/opt/ros/noetic/share/catkin/cmake', '..', 'python'))
try:
    from catkin.environment_cache import generate_environment_script
except ImportError:
    # search for catkin package in all workspaces and prepend to path
    for workspace in '/home/agrotech/fz/devel_isolated/wit_ros_imu;/home/agrotech/fz/devel_isolated/urdf02_gazebo;/home/agrotech/fz/devel_isolated/urdf01_rviz;/home/agrotech/fz/devel_isolated/rplidar_ros;/home/agrotech/fz/devel_isolated/nav_demo;/home/agrotech/fz/devel_isolated/imu_gps_localization;/home/agrotech/fz/devel_isolated/fdilink_ahrs;/home/agrotech/fz/devel_isolated/ekf_localization;/home/agrotech/fz/devel_isolated/arbotix_sensors;/home/agrotech/fz/devel_isolated/arbotix_python;/home/agrotech/fz/devel_isolated/arbotix_msgs;/home/agrotech/fz/devel_isolated/arbotix_firmware;/home/agrotech/fz/devel_isolated/arbotix_controllers;/home/agrotech/fz/devel_isolated/arbotix;/home/agrotech/fz/devel;/opt/ros/noetic'.split(';'):
        python_path = os.path.join(workspace, 'lib/python3/dist-packages')
        if os.path.isdir(os.path.join(python_path, 'catkin')):
            sys.path.insert(0, python_path)
            break
    from catkin.environment_cache import generate_environment_script

code = generate_environment_script('/home/agrotech/fz/devel/env.sh')

output_filename = '/home/agrotech/fz/build/catkin_generated/setup_cached.sh'
with open(output_filename, 'w') as f:
    # print('Generate script for cached setup "%s"' % output_filename)
    f.write('\n'.join(code))

mode = os.stat(output_filename).st_mode
os.chmod(output_filename, mode | stat.S_IXUSR)
