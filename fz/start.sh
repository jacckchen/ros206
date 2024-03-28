#!/bin/bash

gnome-terminal --tab -- bash -c "\
source devel/setup.bash; \
roslaunch urdf01_rviz start.launch; \
exec bash"
 
sleep 5s

gnome-terminal --tab -- bash -c "\
source devel/setup.bash; \
roslaunch ekf_localization ekf_odom.launch; \
exec bash"

sleep 5s

gnome-terminal --tab -- bash -c "\
source devel/setup.bash; \
roslaunch nav_demo nav03_map_server.launch; \
exec bash"

sleep 5s

gnome-terminal --tab -- bash -c "\
source devel/setup.bash; \
roslaunch nav_demo nav04_amcl.launch; \
exec bash"

sleep 5s

gnome-terminal --tab -- bash -c "\
source devel/setup.bash; \
roslaunch nav_demo nav05_path.launch; \
exec bash"

sleep 3s

gnome-terminal --tab -- bash -c "\
rosrun rviz rviz; \
exec bash"

echo "successfully started!"
