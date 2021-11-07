/**
 * File: command.h
 * Desc:
 *    Commands are the objects that control the duration of an
 *    Action/function pointer's execution. An Action may end on
 *    its own, or it may rely on the Command checking its
 *    end_condition. 
 *
 *    Commands can also lead into other Commands. Each additional
 *    Command has a "condition" associated with it that determines 
 *    whether or not that Command will run next. 
 *
 *    Conditions exist as boolean-returning function pointers.
 *
 * Usage:
 *    Command c("Test1");
 *    Action a = DriveAction::drive_forward(drive, 10, 0.5);
 *    c.set_action(a);
 *
 *    Command c2("Test2");
 *    Action a2 = DriveAction::turn_cw(drive, 90, 0.5);
 *    c2.set_action(a2);
 *    c.add_next(c2);
 *
 *    c.execute_all(); 
 */

#ifndef _COMMAND_
#define _COMMAND_

#include "vex.h"
#include "action.h"
#include <stdlib.h>
#include <map>

typedef std::function<bool()> endcond_ptr;
typedef bool (*condition)(void);

class Command 
{
private:

  Action* action;
  endcond_ptr end_condition;

  std::map<Command, condition> next;
  bool is_running;
  std::string name;

  Command **parents;
  int num_parents;

public:
  /**
   * Constructor w/o end_condition: Action runs indefinitely or ends on its own
   */
  Command(std::string _name);

  /**
   * Constructor w/ end_condition
   */
  Command(std::string _name, endcond_ptr _end_condition);

  /**
  * Creates an array of parent Command pointers that is stored in parents.
  * Parent Commands will be checked for completion before executing the
  * Action of the "child" Command.
  *
  * Usage: 
  *    Command *c1;
  *    Command *c2;
  *    make_joiner({c1, c2});
  *
  * @param new_parents - list of Command pointers
  * @return 0 if successful, 1 otherwise
  */
  int make_joiner(std::initializer_list<Command*> new_parents);

  /**
  * Runs the action object stored in this command.
  */
  int execute_action();

  /**
   * Checks the conditions of all "next" Commands. If they return true,
   * run their Actions.
   */
  int check_next();

  /**
   * Runs the Action of the Command it is called on, as well as all
   * subsequent Commands for which their conditions return true
   */
  int execute_all();

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
  void add_next(Command &cmd, condition cond = NULL);

  /**
  * Executes cleanup code which should correspond to the action used
  */
  void term_action();
};

#endif