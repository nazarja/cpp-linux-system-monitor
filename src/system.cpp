#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"


// returns the System cpu
Processor &System::Cpu() { return cpu_; };

// returns a vector of processes sorted by their cpu utilisation
std::vector<Process> &System::Processes()
{
    // clear vector and start fresh
    processes_.clear();
    for (int pid : LinuxParser::Pids()) processes_.emplace_back(Process(pid));

    // lamda used here for sorting uses the overloaded ">" operator from the Process class
    std::sort(processes_.begin(), processes_.end(), [](Process &a, Process &b) -> bool { return a > b;});
    return processes_;
};

// returns system kernel identifier
std::string System::Kernel() { return LinuxParser::Kernel(); };

// returns system memory usage, not including cached memory
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); };

// return os identifier
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); };

// returns number of currently running processes
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); };

// returns total number of alive system processes
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); };

// returns to system uptime since boot
long int System::UpTime() { return LinuxParser::UpTime(); };