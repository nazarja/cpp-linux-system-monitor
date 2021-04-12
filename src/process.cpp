#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return this->pid; };

float Process::CpuUtilization() { return 0.0; };

string Process::Command() { return LinuxParser::Command(this->pid); };

string Process::Ram() { return LinuxParser::Ram(this->pid); };

string Process::User() { return LinuxParser::User(this->pid); };

long int Process::UpTime() { return LinuxParser::UpTime(this->pid); };

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const &a [[maybe_unused]]) const { return true; };