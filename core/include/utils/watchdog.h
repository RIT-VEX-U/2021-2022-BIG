#ifndef _WATCHDOG_
#define _WATCHDOG_

/**
 * Filename: watchdog.h
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

#include "vex.h"

class Watchdog {
  public:
  /**
   * Constructor
   * @param timeout - the number of milliseconds for which the watchdog
   *  should wait
   */
  Watchdog(uint32_t timeout);

  /**
   * Check if the specified amount of time has passed since the 
   * watchdog was initialized
   * @returns true if the amount of time since the watchdog was 
   *  instantiated has met or exceeded the amount of time it was
   *  asked to wait for, false otherwise
   */
  bool is_angry();

  /**
 * Run this function to see watchdog running in terminal
 * NOTE: the controller or brain must be connected to the
 * computer from which the test is being run
 * @param timeout - the number of milliseconds for which you want the 
 *  watchdog to wait
 */
  static void watchdog_test(uint32_t timeout);

  private:
  vex::timer Timer;

  // time (in ms) at which the watch dog instance was initialized
  uint32_t start_time;

  // time (in ms) at which the watch dog instance should
  uint32_t end_time;
};

#endif