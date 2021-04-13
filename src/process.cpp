#include <string>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return this->pid; };

float Process::CpuUtilization() const { return LinuxParser::ProcessCpuUtilisation(this->pid); };

string Process::Command() { return LinuxParser::Command(this->pid); };

string Process::Ram() { return LinuxParser::Ram(this->pid); };

string Process::User() { return LinuxParser::User(this->pid); };

long int Process::UpTime() { return LinuxParser::UpTime(this->pid); };

bool Process::operator>(Process const &a ) const { return CpuUtilization() > a.CpuUtilization(); };