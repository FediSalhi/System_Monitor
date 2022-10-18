#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
    Ram(LinuxParser::Ram(pid_));
    User(LinuxParser::User(pid_));
    RunTime(LinuxParser::ActiveJiffies(pid)/sysconf(_SC_CLK_TCK));
    UpTime(LinuxParser::UpTime(pid_));
    CpuUtilization(CalculateCpuUtilization(pid_));
    Command(LinuxParser::Command(pid_));
}

float Process::CalculateCpuUtilization(int pid) {
    float utilization{0.0};
    long current_runtime{0};
    long current_uptime{0};

    current_runtime = LinuxParser::ActiveJiffies(pid)/sysconf(_SC_CLK_TCK);
    current_uptime = LinuxParser::UpTime(pid);

    // process utilization = total run time / uptime
    if (current_uptime)
    {
        utilization = (float)current_runtime / (float)current_uptime;
    }
    return utilization;
}

int Process::Pid() { return pid_ ; }

float Process::CpuUtilization() { return cpu_utilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

void Process::Pid(int pid) { pid_ = pid;}
   
void Process::User(std::string user) { user_ = user; }

void Process::Command(std::string command) { command_ = command; }

void Process::CpuUtilization(float cpu_utilization) { cpu_utilization_ = cpu_utilization; }

void Process::Ram(std::string ram) { ram_ = ram; }

void Process::UpTime(long int uptime) { uptime_ = uptime; }

void Process::RunTime(long runtime) { runtime_ = runtime; }

bool Process::operator<(Process const& a) const {     
    return this->cpu_utilization_ < a.cpu_utilization_; 
}