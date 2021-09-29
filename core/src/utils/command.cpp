#include "../core/include/utils/command.h"

Command::Command(std::string _name)
  : name(_name) 
{
  printf("command.cpp: Initializing Command with name %s\n", name.c_str());
  fflush(stdout);
  next = NULL;
  end_condition = NULL;
  action = NULL;
  printf("command.cpp: Pointers: %p %p %p\n", next, end_condition, action);
  this->end_condition = [](int arr[]) {
    return false;
  };
  printf("command.cpp: Pointers: %p %p %p\n", next, end_condition, action);
  
}

Command::Command(std::string _name, bool (*_end_condition)(int[]))
  : name(_name), end_condition(_end_condition)
{
  next = NULL;
  printf("command.cpp: Initializing Command with name %s\n", name.c_str());
}

/**
 * Runs the action for this command.
 */
int Command::execute_action() {
  printf("command.cpp: Running action %p in command %s\n", action, name.c_str());
  printf("command.cpp: Pointers: %p %p %p\n", next, end_condition, action);
  int result = action->run(); //Actions should block until complete, so we only run action->run() once
  if (result == 1) { //If the action completed successfully
    printf("command.cpp: Action %p completed successfully in command %s!\n", action, name.c_str());
    if (next != NULL) { //If the next action is set
      printf("command.cpp: Running next command %p from %s!\n", next, name.c_str());
      //return next->execute_action(); //Return the value of the next command
      return true;
    } else { //Otherwise
      printf("command.cpp: No further commands in sequence from %s\n", name.c_str());
      return true; //This command is last in chain, and has completed successfully
    }
  } else {
    printf("command.cpp: Action %p failed in command %s", action, name.c_str());
    return false; //This command has failed
  }
}

/**
 * Sets the action for this command.
 */
void Command::set_action(Action &_action) {
  action = &_action;
  printf("command.cpp: Setting action %p on command %s\n", action, name.c_str());
  printf("command.cpp: Pointers: %p %p %p\n", next, end_condition, action);
}

void Command::set_end_condition(bool (*_end_condition)(int[])) {
 //TODO: Implement End Conditions after a Command supports multiple Actions. 
}

/**
 * Sets the command to run after this one completes successfully.
 */
void Command::add_next(Command &_next) {
  next = &_next;
  printf("command.cpp: Setting next command: %p on command %s\n", next, name.c_str());
  printf("command.cpp: Pointers: %p %p %p\n", next, end_condition, action);
}

/**
  * Executes cleanup code which should correspond to the action used
  */
void Command::term_action()
{
  action->stop();
}

