# ros2_tools

Basic tools for ros2, for emulating ros1 . Requires c++17 to compile.

* Copy the files to a folder PATH_TO_ROSCD

* Compile the cpp with:
`g++ roscd.cpp -o roscd`

* Add this to `.bashrc`:

```
alias roscd=PATH_TO_ROS2TOOLS/roscd
source PATH_TO_ROS2TOOLS/ros2_to_ros1_bash_script.sh
```
