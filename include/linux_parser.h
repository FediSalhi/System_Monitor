#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
#include <vector>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kStatFilePath{"/proc/stat"};
const std::string kUptimeFilePath{"/proc/uptime"};
const std::string kMeminfoFilePath{"/proc/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// generic key-value
template<typename T>
void ExtractValue(const std::string& key, T& value, const std::string& filename);

void ReadAllValues(std::vector<std::string>& values, const std::string& filename);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

// Indexes for CPU utilization per process
#define UTIME_IDX     13
#define STIME_IDX     14
#define CUTIME_IDX    15
#define CSTIME_IDX    16
#define STARTTIME_IDX 21

#define INCLUDE_CHILDREN_PROCESS_TIME_CPU_UTILIZATION

};  // namespace LinuxParser

#endif