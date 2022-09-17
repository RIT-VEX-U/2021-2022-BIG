/**
 * File: command_controller.cpp
 * Desc:
 *    [insert meaningful desc]
 */

#pragma once

#include "../core/include/command_controller.h"
#include "../core/include/delay_command.h"

void CommandController::add(AutoCommand cmd) {
  command_queue.push(cmd);
}

void CommandController::add_delay(int ms) {
  AutoCommand delay = DelayCommand(ms);
  command_queue.push(delay);
}

void CommandController::run() {
  AutoCommand next_cmd;
  while(!command_queue.empty()) {
    next_cmd = command_queue.front();
    command_queue.pop();
  }
}