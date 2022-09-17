#pragma once

#include "./core/include/auto_command.h"

class DelayCommand: public AutoCommand {
  public:

    DelayCommand(int _ms) {
      ms = _ms;
    }
    
    bool run() override {
      vexDelay(ms);
      return true;
    }

  private:

    int ms;
};