/*
    Author: Daniel Rush
    Date: 2021
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "logger.h"
#include "uComs/generated_files/generated_ucoms_decode.h"

const int CMD_LEN = 64;
volatile char CMD_BUF[CMD_LEN] = "\0";
Logger g_log;
uComsDecode g_decoder;

void preParse(int input) {
  static int index = 0;
  switch (input) {
    // Nothing in buffer
    case -1:
      return;
    case '\r':
    case '\n':
      break;
    default:
      CMD_BUF[index] = input;
      index++;
      return;
  }
  if (index > 0) {
    // Call parse on commands with 1 or more char
    g_log.Printf(kLogInfo, "Got: %s", CMD_BUF);
    uComsDecodedCommand cmd = g_decoder.Decode((const char*)CMD_BUF);
    g_log.Printf(kLogInfo, "Decoded:%s", GetDeviceKeyString(cmd.output).c_str());
  }
  index = 0;
  memset((void*)CMD_BUF, '\0', CMD_LEN);
}

int main() {
  stdio_init_all();
  // Wait for a user to be listening(and to send a char)
  getchar();
  g_log.Printf(kLogInfo, "Welcome to Pico Enclosure!");
  // Flush any unused input
  while (true) {
    preParse(getchar_timeout_us(100));
  }
  return 0;
}
