/**
 * File: command_controller.h
 * Desc:
 *    [insert meaningful desc]
 */

#pragma once

#include <queue>
#include "../core/include/auto_command.h"

class CommandController {
  public:

  void add(AutoCommand cmd);
  void add_delay(int ms);
  void run();

  private:
    std::queue<AutoCommand> command_queue;
};