//
// Created by wuyuanyi on 27/01/19.
//

#ifndef CRITICAL_WAIT_FLAGS_CRITICAL_WAIT_FLAGS_H
#define CRITICAL_WAIT_FLAGS_CRITICAL_WAIT_FLAGS_H

#include <atomic>
#include <mutex>
/// A templated class for critical-section-waiting flags. Call enter_critical when the flag may be set in this routine, and exit_critical to release the lock.
/// set_flag can only work after enter critical and before exit critical.
/// set_flag_blocking serves as the short cut to enter critical section -> set_flag -> exit critical section.
/// get_flag will return the current flag value if not in critical section and will block until leaving the critical section, then return the updated flag.
/// get_flag_now will return the current flag regardless it is in critical or not
/// \tparam flag_type
template<typename flag_type>
class critical_wait_flag {
 public:
  explicit critical_wait_flag();
  explicit critical_wait_flag(flag_type initValue);

  void set_flag(flag_type flag);
  void set_flag_blocking(flag_type flag);
  flag_type get_flag();
  flag_type get_flag_now();

  /// Enter the critical section and set the lock. Second call of this function without exit will block the execution
  void enter_critical();

  void exit_critical();
  bool in_critical_section();
 protected:
  flag_type mFlag;
  std::atomic_bool mCriticalFlag;
  std::mutex mCriticalLock;
};

class invalid_set_error : public std::exception {
 public:
  const char *what() const noexcept override;
};


template<typename flag_type>
critical_wait_flag<flag_type>::critical_wait_flag() {
  mCriticalFlag = false;
}
template<typename flag_type>
critical_wait_flag<flag_type>::critical_wait_flag(flag_type initValue) : critical_wait_flag() {
  mFlag = initValue;
}

template<typename flag_type>
void critical_wait_flag<flag_type>::set_flag(flag_type flag) {
  if (!mCriticalFlag) {
    throw invalid_set_error();
  }

  mFlag = flag;
}

template<typename flag_type>
void critical_wait_flag<flag_type>::set_flag_blocking(flag_type flag) {
  enter_critical();
  set_flag(flag);
  exit_critical();
}

template<typename flag_type>
flag_type critical_wait_flag<flag_type>::get_flag() {
  flag_type t;
  mCriticalLock.lock();
  t = mFlag;
  mCriticalLock.unlock();
  return t;
}

template<typename flag_type>
flag_type critical_wait_flag<flag_type>::get_flag_now() {
  return mFlag;
}

template<typename flag_type>
void critical_wait_flag<flag_type>::enter_critical() {
  mCriticalLock.lock();
  mCriticalFlag = true;
}

template<typename flag_type>
void critical_wait_flag<flag_type>::exit_critical() {
  mCriticalFlag = false;
  mCriticalLock.unlock();
}
template<typename flag_type>
bool critical_wait_flag<flag_type>::in_critical_section() {
  return mCriticalFlag;
}

const char *invalid_set_error::what() const noexcept {
  return "flag should be set after entering critical section";
}
#endif //CRITICAL_WAIT_FLAGS_CRITICAL_WAIT_FLAGS_H
