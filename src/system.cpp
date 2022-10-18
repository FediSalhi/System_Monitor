#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::sort;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for(auto pid : pids) {
    Process process(pid);
    processes_.emplace_back(process);
  }
  sort(processes_.begin(), processes_.end(), [](const Process& p1, const Process& p2){
    return p2 < p1;
  });
  return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() { return LinuxParser::UpTime(); }
