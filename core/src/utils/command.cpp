#include "../core/include/utils/command.h"

Command::Command(std::string _name, bool (*_end_condition)(int[]))
  : name(_name), end_condition(_end_condition)
{
}

/**
  * Executes cleanup code which should correspond to the action used
  */
void Command::term_action()
{
  action->stop();
  
}

