#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Generic function to search file for key and return value
std::string LinuxParser::GetStatValue(std::string filepath, std::string param)
{
    std::ifstream filestream(filepath);
    std::string return_value;

    if (filestream.is_open())
    {
        std::string line;
        std::string key;
        std::string value;

        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while(linestream >> key >> value)
            {
                if (key == param)
                {
                    return_value = value;
                    break;
                }
            }
        }
    }
    return return_value;
};

string LinuxParser::OperatingSystem()
{
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value)
            {
                if (key == "PRETTY_NAME")
                {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
};

string LinuxParser::Kernel()
{
    string os, version, kernel;
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
    
    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
};

vector<int> LinuxParser::Pids()
{
    vector<int> pids;
    DIR *directory = opendir(kProcDirectory.c_str());
    struct dirent *file;
    
    while ((file = readdir(directory)) != nullptr)
    {
        // Is this a directory?
        if (file->d_type == DT_DIR)
        {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit))
            {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
};

// without cached memory excluding cached memory
float LinuxParser::MemoryUtilization()
{
    auto get_value = [](std::string value)
    {
        return std::stof(
                LinuxParser::GetStatValue(
                        LinuxParser::kProcDirectory+LinuxParser::kMeminfoFilename, value));
    };

    float total {get_value("MemTotal:")};
    float free {get_value("MemFree:")};
    float buffers {get_value("Buffers:")};
    float cached {get_value("Cached:")};

    return ((total - free) - (buffers + cached)) / total;
};

long LinuxParser::UpTime() {
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
    std::string line;
    long value {0};

    if (filestream.is_open())
    {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> value;
    }
    return value;
};

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; };

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; };

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; };

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; };

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; };

int LinuxParser::TotalProcesses() {
    return std::stoi(
            LinuxParser::GetStatValue(
                    LinuxParser::kProcDirectory + LinuxParser::kStatFilename, "processes"
            ));
};

int LinuxParser::RunningProcesses()
{
    return std::stoi(
            LinuxParser::GetStatValue(
                    LinuxParser::kProcDirectory + LinuxParser::kStatFilename, "procs_running"
                    ));
};

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); };

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); };

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); };

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); };

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; };
