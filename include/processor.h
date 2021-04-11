#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>
#include <string>

class Processor
{
    public:
        float Utilization();


    private:
        std::map<std::string, float> previous {
            {"user", 0.0}, {"nice", 0.0},
            {"system", 0.0}, {"idle", 0.0},
            {"iowait", 0.0}, {"irq", 0.0},
            {"softirq", 0.0}, {"steal", 0.0}
        };

        const std::string jiffy_keys[6] {"user", "nice", "system", "irq", "softirq", "steal"};

};

#endif