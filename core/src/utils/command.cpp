/**
 * File: command.cpp
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

#include "../core/include/utils/command.h"

/**
 * Constructor 1: No end_condition, let Action end on its own
 */
Command::Command(std::string _name)
  : name(_name) 
{
  // Init other values to NULL since an uninitialized pointer
  // is not guaranteed to have any specific value
  action = NULL;
  parents = NULL;

  num_parents = 0;

  this->end_condition = []() {
    return false;
  };
}

/**
 * Constructor 2: end_condition included, end Action when 
 * end_condition returns true
 */
Command::Command(std::string _name, endcond_ptr _end_condition)
  : end_condition(_end_condition), name(_name)
{
  // Init other values to NULL since an uninitialized pointer
  // is not guaranteed to have any specific value
  action = NULL;
  parents = NULL;

  num_parents = 0;
}

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
int Command::make_joiner(std::initializer_list<Command*> new_parents) {
    // err check: has parents already been assigned?
    if(parents != NULL) {
      return 0;
    }

    // flag for when the child Command gets added as "next" to one of its parents
    // (I know this is hack-y, but apparently you can't just get the first element
    // of an initializer_list. If anyone finds a better way, PLEASE replace this :) )
    bool next_set = false;

    // allocating memory to list of parent Commands
    if((parents = (Command**) malloc(sizeof(Command*) * new_parents.size())) == NULL) {
      return 0;
    }

    int index = 0;
    for(auto parent : new_parents){
      parents[index * sizeof(Command*)] = parent;
      num_parents++;

      if(!next_set) {
        parent->add_next(*this);
        next_set = true;
      }
    }

    return 1;
  }

/**
 * Runs the Action for this Command and "next" Commands
 */
int Command::execute_action() {
  for(int i = 0; i < num_parents; i++) {
    // busy loop: stalls while the parent Command is running
    // TODO: might want an "is_finished" field instead in case
    //    the parent hasn't started running
    while(parents[sizeof(Command*) * i]->is_running) {}
  }

  is_running = true;

  // Actions should block until complete, so we only run action->run() once
  if(action->run() < 1) { // error checking
    return 0;
  }

  is_running = false;
  free(parents);
  return 1;
}

/**
 * Checks the conditions of all "next" Commands. If they return true,
 * run their Actions.
 * @return 1 on success, 0 on failure
 */
int Command::check_next() {
  // iterate through each pair of the 
  std::map<Command, condition>::iterator itr;
  for(itr = next.begin(); itr != next.end(); itr++) {
    if(itr->second()) {
      Command c = itr->first;
      c.execute_all();
      // TODO: This is where execution could get nasty depending on synchrony...
    }
  }

  return 1;
}

/**
 * Runs the Action of the Command it is called on, as well as all
 * subsequent Commands for which their conditions return true
 */
int Command::execute_all() {
  for(int i = 0; i < num_parents; i++) {
    // busy loop: stalls while the parent Command is running
    // TODO: might want an "is_finished" field instead in case
    //    the parent hasn't started running
    while(parents[sizeof(Command*) * i]->is_running) {}
  }

  is_running = true;

  if(!action->run()) {
    is_running = false;
    return 0;
  }

  is_running = false;
  free(parents);
  return check_next();
}

/**
 * Sets the action for this command.
 */
void Command::set_action(Action &_action) {
  action = &_action;
}

void Command::set_end_condition(endcond_ptr _end_condition) {
 //TODO: Implement End Conditions after a Command supports multiple Actions. 
}

/**
 * Sets the command to run after this one completes successfully.
 */
void Command::add_next(Command &cmd, condition cond) {
  // if no lead-in condition is given, provide a condition that will always 
  // return true
  if(cond == NULL) {
    cond = [](){return true;};
  }

  next.insert(std::pair<Command, condition>(cmd, cond));
}

/**
  * Executes cleanup code which should correspond to the action used
  */
void Command::term_action()
{
  action->stop();
  free(parents);
}
