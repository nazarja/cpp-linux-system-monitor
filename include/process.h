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
        float CpuUtilization();                 // TODO: See src/process.cpp
        std::string Ram();
        long int UpTime();
        bool operator<(Process const &a) const; // TODO: See src/process.cpp

    private:
        const int pid;
};

#endif