#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <map>
#include <vector>

namespace LinuxParser
{
    // Paths
    const std::string kProcDirectory{"/proc/"};
    const std::string kCmdlineFilename{"/cmdline"};
    const std::string kStatusFilename{"/status"};
    const std::string kStatFilename{"/stat"};
    const std::string kUptimeFilename{"/uptime"};
    const std::string kMeminfoFilename{"/meminfo"};
    const std::string kVersionFilename{"/version"};
    const std::string kOSPath{"/etc/os-release"};
    const std::string kPasswordPath{"/etc/passwd"};
    const std::string jiffy_keys[] {"user", "nice", "system", "idle", "iowait", "irq", "softirq", "steal"};

    // System
    float MemoryUtilization();
    long UpTime();
    std::vector<int> Pids();
    int TotalProcesses();
    int RunningProcesses();
    std::string OperatingSystem();
    std::string Kernel();
    std::string GetStatValue(std::string filepath, std::string param);
    std::string GetFileAsString(std::string filepath);

    // CPU
    std::map<std::string, float> CpuUtilization();

    // Processes
    float ProcessCpuUtilisation(int pid);
    std::string Command(int pid);
    std::string Ram(int pid);
    std::string Uid(int pid);
    std::string User(int pid);
    long int UpTime(int pid);
};

#endif