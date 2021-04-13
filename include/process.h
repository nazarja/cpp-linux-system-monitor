#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process
{
    public:
        explicit Process(int pid) : pid(pid) {};

        int Pid();
        std::string User();
        std::string Command();
        float CpuUtilization() const;
        std::string Ram();
        long int UpTime();
        bool operator>(Process const &a) const;

    private:
        // set process id to easily pass to LinuxParser functions as needed argument
        int pid;
};

#endif