#include <string>
#include <vector>

#include "format.h"

// converts seconds to human readable time format "HH:MM:SS"
std::string Format::ElapsedTime(long seconds)
{
    std::string formatted_time;
    std::vector<long> digits {
        seconds / 3600, // hours
        seconds % 3600 / 60, // minutes
        seconds % 60 // seconds
    };

    int i {0};
    for (long digit : digits) {
        std::string zero{"0"};
        std::string value = std::to_string(digit);

        // if a single digit, prepend with a leading "0"
        formatted_time += value.length() == 1 ? zero.append(value) : value;

        // if not the last unit, append a colon ":"
        if (i++ != 2) formatted_time += ":";
    }
    return formatted_time;
};