#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    string formatted_output{""};

    int hours = seconds / 3600;
    int minutes = (seconds % 3600)/60;
    int sec = seconds % 60;

    string hours_str;
    string minutes_str;
    string seconds_str;

    if (hours<10) {
        hours_str = "0"+ std::to_string(hours);
    } else {
        hours_str = std::to_string(hours);
    }
    if (minutes<10) {
        minutes_str = "0"+ std::to_string(minutes);
    } else {
        minutes_str = std::to_string(minutes);
    }
    if (sec<10) {
        seconds_str = "0"+ std::to_string(sec);
    } else {
        seconds_str = std::to_string(sec);
    }
    formatted_output = hours_str + ':' + minutes_str + ':' + seconds_str; 

    return formatted_output; 
    }