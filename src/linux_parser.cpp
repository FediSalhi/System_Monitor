#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE (FEDI): Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::ifstream meminfo_fs(kMeminfoFilePath);
  string line;
  string key;
  string value;
  string memtotal;
  string memfree;
  float memtotal_f = 0.0;
  float memfree_f = 0.0;
  if (meminfo_fs.is_open())
  {
    while (getline(meminfo_fs, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream line_fs(line);
      line_fs >> key >> value;
      if (key == "MemTotal") {
        memtotal = value;
        memtotal_f = stof(memtotal);
      } else  if (key == "MemFree") {
        memfree = value;
        memfree_f = stof(memfree);
        break;
      }
    }
  } else {
    std::cout << "Error reading file: " + kMeminfoFilePath << std::endl;
    exit(-1);
  }
  
  return (memtotal_f - memfree_f)/memtotal_f;
}

// DONE (FEdi): Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream uptime_fs(kUptimeFilePath);
  string line;
  string uptime;
  string idletime;
  if (uptime_fs.is_open()) {
    if (getline(uptime_fs, line)) {
      std::istringstream line_ss(line);
      line_ss >> uptime >> idletime;
    }
  } else {
    std::cout << "Error reading file: " + kUptimeFilePath << std::endl;
    exit(-1);
  }
  return stol(uptime);
}

// DONE (Fedi): Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// DONE (fedi): Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  // nonIdle = user + nice + system + irq + softirq + steal
  std::ifstream stat_fs(kStatFilePath);
  string line;
  vector<string> values;
  string value;
  long user = 0;
  long nice = 0;
  long system = 0;
  long irq = 0;
  long softirq = 0;
  long steal = 0;
  long total_non_idle = 0;
  if (stat_fs.is_open()) {
    while(getline(stat_fs, line)) {
      if(line.find("cpu ") != string::npos) {
        break;
      }
    }
    std::istringstream line_ss(line);
    while (line_ss) {
      line_ss >> value;
      values.emplace_back(value);
    }
    user = stol(values[CPUStates::kUser_ + 1]);
    nice = stol(values[CPUStates::kNice_ + 1]);
    system = stol(values[CPUStates::kSystem_ + 1]);
    irq = stol(values[CPUStates::kIRQ_ + 1]);
    softirq = stol(values[CPUStates::kSoftIRQ_ + 1]);
    steal = stol(values[CPUStates::kSteal_ + 1]);
    total_non_idle = user + nice + system + irq + softirq + steal;
  } else {
    std::cout << "Error reading file: " + kStatFilePath << std::endl;
    exit(-1);
  }
  return total_non_idle; }

// DONE (fedi): Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  // idle = idle + iowait
  std::ifstream stat_fs(kStatFilePath);
  string line;
  vector<string> values;
  string value;
  long idle = 0;
  long iowait = 0;
  long total_idle = 0;
  if (stat_fs.is_open()) {
    while(getline(stat_fs, line)) {
      if(line.find("cpu ") != string::npos) {
        break;
      }
    }
    std::istringstream line_ss(line);
    while (line_ss) {
      line_ss >> value;
      values.emplace_back(value);
    }
    idle = stol(values[CPUStates::kIdle_ + 1]);
    iowait = stol(values[CPUStates::kIOwait_ + 1]);
    total_idle = idle + iowait;
  } else {
    std::cout << "Error reading file: " + kStatFilePath << std::endl;
    exit(-1);
  }
  return total_idle; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE (Fedi): Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stat_fs(kStatFilePath);
  string line;
  string key;
  string value;
  if (stat_fs.is_open()) {
    while (getline(stat_fs, line)) {
      std::istringstream line_ss(line);
      line_ss >> key >> value;
      if (key == "processes") {
        break;
      }
    }
  } else {
    std::cout << "Error reading file: " + kStatFilePath << std::endl;
    exit(-1);
  }
  return stoi(value);
}

// DONE (Fedi): Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stat_fs(kStatFilePath);
  string line;
  string key;
  string value;
  if (stat_fs.is_open()) {
    while (getline(stat_fs, line)) {
      std::istringstream line_ss(line);
      line_ss >> key >> value;
      if (key == "procs_running") {
        break;
      }
    }
  } else {
    std::cout << "Error reading file: " + kStatFilePath << std::endl;
    exit(-1);
  }
  return stoi(value);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
