#include <string>
#include <map>

#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() {
    std::map<std::string, float> current = LinuxParser::CpuUtilization();

    float prev_idle { previous["idle"] + previous["iowait"] };
    float curr_idle { current["idle"] + current["iowait"] };;

    float prev_non_idle {0};
    float curr_non_idle {0};

    for (int i = 0; i < 2; i++)
    {
        for (std::string key : Processor::jiffy_keys)
        {
            i == 0 ? prev_non_idle += previous[key] : curr_non_idle += current[key];
        }
    }

    float prev_total { prev_idle + prev_non_idle };
    float curr_total { curr_idle + curr_non_idle };;

    float diff_total {  curr_total - prev_total };
    float diff_idle {  curr_idle - prev_idle };

    Processor::previous = current;

    return (diff_total - diff_idle) / diff_total;
};