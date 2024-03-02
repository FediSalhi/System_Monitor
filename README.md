# System Monitor

This project is a simpler version of the Linux system monitoring tool [htop](https://htop.dev/) that was built for the Udacity C++ Nano Degree program second course. The tool shows information related to the system and the proccesses, including:
- Operating system name and version
- Kernel version
- Average CPU utilization
- Average RAM utilization
- Total number of processes
- Number of running processes
- System Up Time
- List of 10 processes with the highest CPU utilization. The list includes:
    - Process ID 
    - User name
    - CPU core utilization
    - RAM
    - Up Time
    - Command used to run the process

The project relies on [ncurses](https://www.gnu.org/software/ncurses/), a library that facilitates text-based graphical output in the terminal.

<p align="center">
<img align="center" src="https://user-images.githubusercontent.com/45536639/196511106-2b5b4636-00bb-42a9-b154-a3411ab49c5f.png"> 
</p>


## Dependencies (Linux)
1. Cmake and GCC:
```
    sudo apt update
    sudo apt install build-essential
    sudo apt install cmake
```

2. Ncurses [Ncurses](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/intro.html) Library
```
sudo apt install libncurses5-dev libncursesw5-dev
```

## Build
To Build the project, first, create a `build` directory and change to that directory:
```
    mkdir build && cd build
```
From within the `build` directory, run `cmake` and `make` as follows:
```
    cmake ..
    make
```
## Running
The executable will be placed in the `build` directory. From within `build`, you can run the project as follows:
```
    ./monitor
```

## Authors

* **Fedi Salhi** [FediSalhi](https://www.linkedin.com/in/fedisalhi/)
