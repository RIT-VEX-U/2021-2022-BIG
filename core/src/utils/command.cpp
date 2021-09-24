#include "../core/include/utils/command.h"

Command::Command(std::string _name, bool (*_end_condition)(int[]))
  : name(_name), end_condition(_end_condition)
{
  printf("command.cpp: Initializing Command with name %s\n", name.c_str());
}

/**
 * Runs the action for this command.
 */
int Command::execute_action() {
  printf("command.cpp: Running action %p\n", action);
  return action->run();
}

/**
 * Sets the action for this command.
 */
void Command::set_action(Action _action) {
  action = &_action;
  printf("command.cpp: Setting action %p\n", action);
}

void Command::set_end_condition(bool (*_end_condition)(int[])) {
 //TODO: Implement End Conditions after a Command supports multiple Actions. 
}

/**
  * Executes cleanup code which should correspond to the action used
  */
void Command::term_action()
{
  action->stop();
}

