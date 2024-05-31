# ros2_tools

Basic tools for ros2, for emulating ros1 tools

* Copy the files to a folder PATH_TO_ROSCD

* Compile the cpp with:
`g++ roscd.cpp -o roscd`

* Add this to `.bashrc`:

```
alias roscd=PATH_TO_ROSCD/roscd
source PATH_TO_ROSCD/roscd.sh
```
