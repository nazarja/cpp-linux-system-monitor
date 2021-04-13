#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Generic function to search file for a key and return value
// takes a filepath and a key to search for as parameters
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
                // if we have found our key, break the loop to prevent unnecessary operations
                if (key == param)
                {
                    return_value = value;
                    break;
                }
            }
        }
    }

    // return the found value or an empty string
    return return_value;
};

// returns the first line of a file
// will return entire file as a string if the file is only a single line
std::string LinuxParser::GetFileAsString(std::string filepath)
{
    std::ifstream filestream(filepath);
    std::string line {""};

    if (filestream.is_open()) std::getline(filestream, line);
    return line;
};

// parses and returns os identifier
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

// parses and returns kernel identifier
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

// creates a vector of process ids based on process dirs in /proc
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

// return aggregate system memory usage excluding cached memory
float LinuxParser::MemoryUtilization()
{
    // lambda: cut down on code for calling GetStatValue
    auto get_value = [](std::string value)
    {
        return std::stof(
                LinuxParser::GetStatValue(
                        LinuxParser::kProcDirectory+LinuxParser::kMeminfoFilename, value));
    };

    // memory values - uses: lambda get_value
    float total {get_value("MemTotal:")};
    float free {get_value("MemFree:")};
    float buffers {get_value("Buffers:")};
    float cached {get_value("Cached:")};

    // return calculation
    return ((total - free) - (buffers + cached)) / total;
};

// parse and return system uptime
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

// parse and return system aggregate cpu utilisation
std::map<std::string, float> LinuxParser::CpuUtilization() {
    // only get first line of file
    std::string line{LinuxParser::GetFileAsString(LinuxParser::kProcDirectory + LinuxParser::kStatFilename)};
    std::map<std::string, float> states;

    std::istringstream linestream(line);
    std::string value;

    // iterate over keys of jiffies, add value to map of states that will be returned to caller
    for (std::string key : LinuxParser::jiffy_keys)
    {
        linestream >> value;

        // add each key except first stream
        if (value != "cpu") states[key] = std::stof(value);
    }
    return states;
};

// parse and return total number of processes
int LinuxParser::TotalProcesses() {
    return std::stoi(
            LinuxParser::GetStatValue(
                    LinuxParser::kProcDirectory + LinuxParser::kStatFilename, "processes"
            ));
};

// parse and return all running processes
int LinuxParser::RunningProcesses()
{
    return std::stoi(
            LinuxParser::GetStatValue(
                    LinuxParser::kProcDirectory + LinuxParser::kStatFilename, "procs_running"
                    ));
};

// parse and return caommand used to start process
std::string LinuxParser::Command(int pid)
{
    return LinuxParser::GetFileAsString(
            LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
};

// parse and return ram usage by process
std::string LinuxParser::Ram(int pid)
{
    std::string filepath { LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename };
    std::string value = LinuxParser::GetStatValue(filepath, "VmSize:");

    if (value != "")
    {
        float mb = std::stof(value) / 1000;

        // https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
        std::stringstream mb_stream;

        // if the float is greater tha 1.0, show as a whole number, less than one show as fractional number
        mb_stream << std::fixed << std::setprecision(mb > 0 ? 0 : 2) << mb;
        return mb_stream.str();
    }
    return "";
};

// parse and return uid integer - will be called by LinuxParser::User
std::string LinuxParser::Uid(int pid)
{
    std::string filepath { LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename };
    std::string uid = GetStatValue(filepath, "Uid:");
    return uid;
};

// calls Uid, receives integer and parses /etc/passwd for user associated with uid
std::string LinuxParser::User(int pid)
{
    std::string uid  = LinuxParser::Uid(pid);
    std::string user, x, user_uid;
    std::ifstream filestream(LinuxParser::kPasswordPath);

    if (filestream.is_open())
    {
        std::string line;
        while(std::getline(filestream, line))
        {
            // replcae colon with spaces in order to stream file easily
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);

            while (linestream >> user >> x >> user_uid)
            {
                 // we want the user_id match the uid
                 if (user_uid == uid) {

                     // if the username is greater than 6 chars create substring
                     // this prevents username overlapping / no spaces between the user and ram sections in the interface
                     return user.length() > 6
                        ? user.substr(0, 6)
                        : user;
                 }
            }
        }
    }
    return std::string();
};

// parse and return the system uptime
long LinuxParser::UpTime(int pid)
{
    std::string filepath { LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename };
    std::string line = LinuxParser::GetFileAsString(filepath);
    std::string value;

    std::istringstream linestream(line);
    int counter = 0;

    while(linestream >> value && ++counter)
    {
        // we only want the 22nd
        if (counter == 22)
            // calculation converts ticks to seconds and is subtracted from system uptime
            return LinuxParser::UpTime() - (std::stol(value) / sysconf(_SC_CLK_TCK));
    }
    return 0;
};

// parse, calculate and return aggregate cpu usage by a process
float LinuxParser::ProcessCpuUtilisation(int pid)
{
    std::string fileline = LinuxParser::GetFileAsString(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
    std::istringstream linestream(fileline);
    std::map<std::string, long> values;
    std::string value;
    short count {-1};

    while(linestream >> value)
    {
        // stop iterating after 22, no useful strings
        if (++count >= 22) break;

        // we only want certain values
        switch(count)
        {
            case 13:
                values["utime"] = std::stol(value);
                break;
            case 14:
                values["stime"] = std::stol(value);
                break;
            case 15:
                values["cutime"] = std::stol(value);
                break;
            case 16:
                values["cstime"] = std::stol(value);
                break;
            case 21:
                values["starttime"] = std::stol(value);
                break;
            default:
                break;
        }
    }

    // calculations based on https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    long total_time = (values["utime"]+values["stime"]+values["cutime"]+values["cstime"]);
    float seconds = LinuxParser::UpTime() - (values["starttime"] / sysconf(_SC_CLK_TCK));

    return  ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
};