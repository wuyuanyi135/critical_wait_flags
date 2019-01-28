//
// Created by wuyuanyi on 28/01/19.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include "critical_wait_flags.h"


void should_block_when_read_critical() {
  critical_wait_flag<bool> flag(false);
  bool hasExited = false;

  // initially should be false
  assert(!flag.get_flag());

  flag.enter_critical();

  std::thread readThread([&flag, &hasExited](){
    assert(flag.get_flag());
    assert(hasExited);
  });

  std::this_thread::sleep_for(std::chrono::seconds(2));
  flag.set_flag(true);
  hasExited = true;
  flag.exit_critical();
  readThread.join();
}

int main() {
  should_block_when_read_critical();
  return 0;
}