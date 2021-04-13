#include <string>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// returns the processes identifier
int Process::Pid() { return this->pid; };

// returns the cost of cpu usage by the process as a percentage
float Process::CpuUtilization() const { return LinuxParser::ProcessCpuUtilisation(this->pid); };

// retruns the command used to start the process
string Process::Command() { return LinuxParser::Command(this->pid); };

// returns the memory used by the process
string Process::Ram() { return LinuxParser::Ram(this->pid); };

// returns the user associated with the process
string Process::User() { return LinuxParser::User(this->pid); };

// returns the process uptime against the system boot time
long int Process::UpTime() { return LinuxParser::UpTime(this->pid); };

// overloaded operator tests this Process's cpu utilisation against another. Sort order should be descending.
bool Process::operator>(Process const &a ) const { return CpuUtilization() > a.CpuUtilization(); };