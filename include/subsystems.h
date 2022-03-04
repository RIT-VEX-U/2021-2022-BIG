#pragma once
#include "robot-config.h"
#include <atomic>

namespace front_claw
{
  extern bool is_open;
  bool open();
  bool close();
  void control(bool toggle);
}

namespace rear_claw
{
  extern bool is_open;
  bool open();
  bool close();
  void control(bool toggle);
}

namespace conveyor
{
  extern vex::task *task_handle;
  extern std::atomic<bool> is_running;

  void initialize();
  bool start();
  bool stop();
  void control(bool toggle);
}