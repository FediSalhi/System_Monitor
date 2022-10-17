#include "ncurses_display.h"
#include "system.h"

//debug
#include "linux_parser.h"

int main() {
  System system;
 // system.Cpu().Utilization();
  NCursesDisplay::Display(system);
}