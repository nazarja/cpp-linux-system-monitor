#include <string>
#include <vector>

#include "format.h"

std::string Format::ElapsedTime(long seconds)
{
    std::string formatted_time {""};
    std::vector<long> digits {(seconds / 3600), ((seconds % 3600) / 60), (seconds % 60)}; // hours, minutes, seconds

    int i {0};
    for (long digit : digits) {
        std::string zero{"0"};
        std::string value = std::to_string(digit);
        formatted_time += value.length() == 1 ? zero.append(value) : value;
        if (i++ != 2) formatted_time += ":";
    }
    return formatted_time;
};