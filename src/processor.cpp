#include "processor.h"

Processor::Processor() : idle_jiffies(LinuxParser::IdleJiffies()), active_jiffies(LinuxParser::ActiveJiffies()) {}

void Processor::ActiveJiffies(long active_jiffies) { this->active_jiffies = active_jiffies; } 

void Processor::IdleJiffies(long idle_jiffies) {this->idle_jiffies = idle_jiffies; }

long Processor::ActiveJiffies() { return active_jiffies; }

long Processor::IdleJiffies() { return idle_jiffies; }

float Processor::Utilization() {
    float utilization{0.0};

    // calculate delta jiffies (delta time = 1s)
    long d_idle = LinuxParser::IdleJiffies() - IdleJiffies();
    long d_active = LinuxParser::ActiveJiffies() - ActiveJiffies(); 

    if (d_idle + d_active) // prevents dividing by zero
    {
        utilization = (float)d_active / ((float)d_active + (float)d_idle);
    }

    //update the jiffies for next read
    IdleJiffies(LinuxParser::IdleJiffies());
    ActiveJiffies(LinuxParser::ActiveJiffies());

    return utilization;
}