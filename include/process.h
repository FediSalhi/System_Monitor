#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <vector>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  //Accessors
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
    
  //Mutators
  void Pid(int pid);
  void User(std::string user);
  void Command(std::string command);
  void CpuUtilization(float cpu_utilization);
  void Ram(std::string ram);
  void UpTime(long int uptime);
  void RunTime(long runtime);
  void IdleJiffies(long idle_jiffies);
  void ActiveJiffies(long active_jiffies);

  //helper methods
  float CalculateCpuUtilization(int pid);
  bool operator<(Process const& a) const;

 private:
  int pid_;
  long runtime_;
  long uptime_;
  float cpu_utilization_;
  std::string user_;
  std::string command_;
  std::string ram_;
};

#endif