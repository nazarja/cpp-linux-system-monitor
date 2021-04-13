# Cpp-Linux-System-Monitor

A C++ 17 project that creates a system monitor similar to htop, using ncurses to display out to the terminal.

The main uses for this program is to make use of an object orientated approach to programming in c++, extensive file parsing and string streaming is implemented. 
During this project I gained extra knowledge with the /proc dir in Linux, calculating aggregate values for process memory and cpu utilisation.

Also creating operator overloads, working with vectors maps and streamstreams, lambdas, header files, namespaces and classes.

## Completed Screenshot

![system monitor](images/sysmonitor.png?raw=true "Completed System Monitor")

# Installation / Build

- `git clone https://github.com/nazarja/cpp-linux-system-monitor.git`  
  Unzip and cd to project folder location
- `mkdir build && cd build`
- `cmake ..`
- `make`
  
Next command launches and runs program in terminal
- `./monitor`


