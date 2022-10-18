#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor();
  void IdleJiffies(long idle_jiffies);
  void ActiveJiffies(long active_jiffies);
  long IdleJiffies();
  long ActiveJiffies();
  float Utilization();

 private:
 long idle_jiffies{0};
 long active_jiffies{0};
};

#endif