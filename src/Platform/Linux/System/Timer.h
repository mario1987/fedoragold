// Copyright (c) 2011-2016 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <chrono>
#include <errno.h>
#include <System/ErrorMessage.h>

// for edge case on Android
#ifndef EAGAIN
#define EAGAIN 35
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK 11
#endif

namespace System {

class Dispatcher;

class Timer {
public:
  Timer();
  explicit Timer(Dispatcher& dispatcher);
  Timer(const Timer&) = delete;
  Timer(Timer&& other);
  ~Timer();
  Timer& operator=(const Timer&) = delete;
  Timer& operator=(Timer&& other);
  void sleep(std::chrono::nanoseconds duration);

private:
  Dispatcher* dispatcher;
  void* context;
  int timer;

  bool testerrno(int errnm) {
    if (errnm != EAGAIN) {
      if (errnm != EWOULDBLOCK) {
        return false;
      }
    }
    return true;
  }
};

}
