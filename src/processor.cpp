#include <string>
#include <map>

#include "linux_parser.h"
#include "processor.h"

// calculates average cpu utilisation, returns as a percentage
float Processor::Utilization() {
    // gets map of cpu jiffies as key value pairs
    std::map<std::string, float> current = LinuxParser::CpuUtilization();

    // idles
    float prev_idle { previous["idle"] + previous["iowait"] };
    float curr_idle { current["idle"] + current["iowait"] };;

    // non idles
    float prev_non_idle {0};
    float curr_non_idle {0};

    // calculates prev_non_idle & curr_non_idle
    for (int i = 0; i < 2; i++)
    {
        // iterate over jiffy_key increments prev or curr non_idle
        for (const std::string& key : Processor::jiffy_keys)
        {
            // if on first loop, increment prev else increment curr non idles
            i == 0 ? prev_non_idle += previous[key] : curr_non_idle += current[key];
        }
    }

    // totals
    float prev_total { prev_idle + prev_non_idle };
    float curr_total { curr_idle + curr_non_idle };;

    // differences
    float diff_total {  curr_total - prev_total };
    float diff_idle {  curr_idle - prev_idle };

    // assign current processes map to previous for next update calculation
    Processor::previous = current;

    return (diff_total - diff_idle) / diff_total;
};