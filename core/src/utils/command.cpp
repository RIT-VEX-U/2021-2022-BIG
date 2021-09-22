#include "../core/include/utils/command.h"

Command::Command(std::string _name, bool (*_end_condition)(int[]))
  : name(_name), end_condition(_end_condition)
{
}

