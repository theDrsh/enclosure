// Author: Daniel Rush
#pragma once
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"

enum LoggerLevel {
  kLogDebug  = 0,
  kLogInfo   = 1,
  kLogError  = 2,
  kLogFatal  = 3,
  kLogLevels,
};

const int32_t MSG_SIZE = 256;

class Logger {
 public:
  void Printf(LoggerLevel Level, const char* format, ...);
 private:
  void Write(const char* msg);
  void FormatMessage(LoggerLevel level, const char* format, va_list args);
  int foo;
  char msg_buffer_[MSG_SIZE];
};
