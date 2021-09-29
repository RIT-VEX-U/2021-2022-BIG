/**
 * Filename: watchdog.cpp
 * Author: Dana Colletti
 * Desc:
 *    Watchdog makes use of the Timer class in the VEX API
 *    to alert the function that called it when the specified amount
 *    of time has passed
 *
 * Usage:
 *    Initialize the watchdog:
 *      Watchdog w(1000);
 *    Check if the timeout time has been reached:
 *      w.is_angry()
 */

#include "../core/include/utils/watchdog.h"
#include <iostream>

// Constructor
Watchdog::Watchdog(uint32_t timeout) {
  Timer = vex::timer();
  start_time = Timer.time();
  end_time = start_time + timeout;
}

/**
 * Check if the specified amount of time has passed since the 
 * watchdog was initialized
 * @returns true if the amount of time since the watchdog was 
 *  instantiated has met or exceeded the amount of time it was
 *  asked to wait for, false otherwise
 */
bool Watchdog::is_angry() {
  return Timer.time() >= end_time;
}

/**
 * Run this function to see watchdog running in terminal
 * NOTE: the controller or brain must be connected to the
 * computer from which the test is being run
 * @param timeout - the number of milliseconds for which you want the 
 *  watchdog to wait
 */
void Watchdog::watchdog_test(uint32_t timeout) {
  vex::timer T2 = vex::timer();
  Watchdog w(timeout);
  while(!w.is_angry()) {
    std::cout << "Watchdog is watching  " << "Time: " << T2.time() << "\n";
    vexDelay(20);
  }

  std::cout << "WATCHDOG IS ANGRY" << std::endl;
}