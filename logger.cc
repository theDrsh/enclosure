#include "logger.h"


static const char* LoggerLevelStrings[kLogLevels] = {
  [kLogDebug] = "D",
  [kLogInfo]  = "I",
  [kLogError] = "E",
  [kLogFatal] = "F",
};

void Logger::Printf(LoggerLevel level, const char* format, ...) {
  va_list args;
  va_start(args, format);
  FormatMessage(level, format, args);
  Write(msg_buffer_);
}

void Logger::Write(const char* msg) {
  printf("%s", msg);
}

void Logger::FormatMessage(LoggerLevel level, const char* format, va_list args) {
  uint64_t ticks = get_absolute_time();
  int milliseconds = ticks % 1000;
  int seconds = ticks / 1000;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  char tmp_buffer[MSG_SIZE] = "";
  if ((level < kLogLevels) && (level > kLogDebug)) {
    snprintf(tmp_buffer, MSG_SIZE, "%.2d:%.2d:%2d.%.3d %s: %s\n", hours,
                                                               minutes,
                                                               seconds,
                                                               milliseconds,
                                                               LoggerLevelStrings[level],
                                                               format);
  }
  vsnprintf(msg_buffer_, MSG_SIZE, tmp_buffer, args);
}
