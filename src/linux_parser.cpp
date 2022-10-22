#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template<typename T>
void LinuxParser::ExtractValue(const std::string& key, T& value, const std::string& filename) {
  std::ifstream filestream(filename);
  std::string line;
  std::string key_tmp;
  T value_tmp;
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      if (key == "PRETTY_NAME") { std::replace(line.begin(), line.end(), ' ', '_'); }
      else if (key == "procs_running") {}
      else { std::replace(line.begin(), line.end(), '_', ' '); }
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream line_ss(line);
      if (line_ss) {
        line_ss >> key_tmp >> value_tmp;
        if (key_tmp == key) {
          value = value_tmp;
          break;
        }
      }
    }
    filestream.close();
  }
}

void LinuxParser::ReadAllValues(std::vector<std::string>& values, const std::string& filename) {
  std::ifstream filestream(filename);
  std::string line;
  std::string value;
  if(filestream.is_open()) {
    getline(filestream, line);
    if (!line.empty()) {
      std::istringstream line_ss(line);
      while(line_ss) {
        line_ss >> value;
        values.emplace_back(value);
      }
    }
    filestream.close();
  }
}

string LinuxParser::OperatingSystem() {
  string value;
  ExtractValue<std::string>(std::string("PRETTY_NAME"), value, kOSPath);
  std::replace(value.begin(), value.end(), '_', ' ');
  return value;
}

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

// TODO: Re-implement using std::filesystem
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

float LinuxParser::MemoryUtilization() { 
  float memtotal_f{0.0};
  float memfree_f{0.0};

  ExtractValue<float>(std::string("MemTotal"), memtotal_f, kMeminfoFilePath);
  ExtractValue<float>(std::string("MemFree"), memfree_f, kMeminfoFilePath);

  return (memtotal_f - memfree_f)/memtotal_f;
}

long LinuxParser::UpTime() {
  std::ifstream uptime_fs(kUptimeFilePath);
  string line;
  string uptime;
  string idletime;
  long uptime_l{0};
  if (uptime_fs.is_open()) {
    if (getline(uptime_fs, line)) {
      std::istringstream line_ss(line);
      line_ss >> uptime >> idletime;
      uptime_l = stol(uptime);
    }
  }
  return uptime_l;
}

long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> values;
  long utime{0};            //cpu time spent in user mode (in clock ticks)
  long stime{0};            //cpu time spent in kernel model (int clock ticks)
  long cutime{0};           //Waited-for children's CPU time spent in user code (in clock ticks)
  long cstime{0};           //Waited-for children's CPU time spent in kernel code (in clock ticks)
  long total_non_idle{0};

  ReadAllValues(values, kProcDirectory + to_string(pid) + kStatFilename);
  if (!values.empty()) {
    utime = stol(values[UTIME_IDX]);
    stime = stol(values[STIME_IDX]);
    cutime = stol(values[CUTIME_IDX]);
    cstime = stol(values[CSTIME_IDX]);
    #ifdef INCLUDE_CHILDREN_PROCESS_TIME_CPU_UTILIZATION
    total_non_idle = utime + stime + cutime + cstime;
    #else
    total_non_idle = utime + stime ;
    #endif
  }
  return total_non_idle; 
}

long LinuxParser::ActiveJiffies() { 
  // nonIdle = user + nice + system + irq + softirq + steal
  std::ifstream stat_fs(kStatFilePath);
  string line;
  vector<string> values;
  string value;
  long user{0};
  long nice{0};
  long system{0};
  long irq{0};
  long softirq{0};
  long steal{0};
  long total_non_idle{0};

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
  }
  return total_non_idle; 
  }

long LinuxParser::IdleJiffies() { 
  // idle = idle + iowait
  std::ifstream stat_fs(kStatFilePath);
  string line;
  vector<string> values;
  string value;
  long idle{0};
  long iowait{0};
  long total_idle{0};

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
  }
  return total_idle;
  }

int LinuxParser::TotalProcesses() {
  int processes{0};
  ExtractValue<int>(std::string("processes"), processes, kStatFilePath);
  return processes;
}

int LinuxParser::RunningProcesses() {
  int processes{0};
  ExtractValue<int>(std::string("procs_running"), processes, kStatFilePath);
  return processes;
}

string LinuxParser::Command(int pid) { 
  std::ifstream proc_fs(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  string command;
  if(proc_fs.is_open()) {
    if(getline(proc_fs, line)) {
      command = line;
    }
  }
  return command;
}

string LinuxParser::Ram(int pid) { 
  int mem_utilization_kb;
  int mem_utilization_mb;
  ExtractValue<int>(std::string("VmSize"), mem_utilization_kb, kProcDirectory + to_string(pid) + kStatusFilename);
  mem_utilization_mb = mem_utilization_kb/1000;
return to_string(mem_utilization_mb); 
}

string LinuxParser::Uid(int pid) { 
  string value;
  ExtractValue<std::string>(std::string("Uid"), value, kProcDirectory + to_string(pid) + kStatusFilename);
  return value; 
}

string LinuxParser::User(int pid){ 
  std::ifstream passwd_fs(kPasswordPath);
  string line;
  string user;
  string uid{Uid(pid)};

  if(passwd_fs.is_open()) {
    while(getline(passwd_fs, line)) {
      if (line.find(uid) != string::npos) 
      {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream line_ss(line);
        line_ss >> user;
      }
    }
  }     
  return user;
}

long LinuxParser::UpTime(int pid) { 
  vector<string> values;
  long starttime{0};
  long uptime{0};

  ReadAllValues(values, kProcDirectory + to_string(pid) + kStatFilename);
  if (!values.empty()) {
    starttime = stol(values[STARTTIME_IDX])/sysconf(_SC_CLK_TCK);
    uptime = UpTime() - starttime;
  }
return uptime;
}


