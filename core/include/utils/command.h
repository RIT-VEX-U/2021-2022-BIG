#ifndef _COMMAND_
#define _COMMAND_

#include "vex.h"
#include "action.h"
#include <stdlib.h>

typedef std::function<bool()> endcond_ptr;

class Command 
{
private:
  Action* action;
  endcond_ptr end_condition;

  Command* next;
  bool is_running;
  std::string name;

public:
  Command(std::string _name);
  Command(std::string _name, endcond_ptr _end_condition);

  /**
  * Runs the action object stored in this command.
  */
  int execute_action();

  /**
  * Assigns an action to store in the command to be run.
  * @param _action
  *   The action to store.
  */
  void set_action(Action &_action);

  /**
  * Assigns the set of conditions that, when satisfied, ends the command.
  * The array accommodates multiple conditions for OR and AND logic.
  * @param (*_end_condition)(int[])
  *   An array of (two) conditions that the command may meet.
  */
  void set_end_condition(endcond_ptr _end_condition);

  /**
  * Assigns the command to be run when this command ends.
  * @param _next
  *   A Command object to queue.
  */
  void add_next(Command &_next);

  /**
  * Executes cleanup code which should correspond to the action used
  */
  void term_action();
};

#endif