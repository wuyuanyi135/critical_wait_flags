//
// Created by wuyuanyi on 28/01/19.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <cassert>
#include "critical_wait_flags.h"

void should_block_when_read_critical() {
  critical_wait_flag<bool> flag(false);
  bool hasExited = false;

  // initially should be false
  assert(!flag.get_flag());

  flag.enter_critical();

  std::thread readThread([&flag, &hasExited]() {
    assert(flag.get_flag());
    assert(hasExited);
  });

  std::this_thread::sleep_for(std::chrono::seconds(1));
  flag.set_flag(true);
  hasExited = true;
  flag.exit_critical();
  readThread.join();
}

void should_set_blocking_same_as_set_in_critical() {
  const std::future<void> &future = std::async(std::launch::async, []() {
    critical_wait_flag<bool> flag(false);
    flag.enter_critical();
    flag.set_flag_blocking(true);
    assert(flag.get_flag_now());
    flag.exit_critical();
    assert(flag.get_flag());
  });

  std::future_status status = future.wait_for(std::chrono::seconds(1));
  if (status == std::future_status::timeout) {
    std::cerr << "test failed in should_set_blocking_same_as_set_in_critical: timeout" << std::endl;
    return;
  }
}

int main() {
  should_block_when_read_critical();
  should_set_blocking_same_as_set_in_critical();
  return 0;
}