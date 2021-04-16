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
const uint LED_PIN = 25;

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
      if(index > CMD_LEN) {
        index = 0;
        memset((void*)CMD_BUF, '\0', CMD_LEN);
        break;
      }
      CMD_BUF[index] = input;
      index++;
      return;
  }
  if (index > 0) {
    // Call parse on commands with 1 or more char
    g_log.Printf(kLogInfo, "Got: %s", CMD_BUF);
    uComsDecodedCommand cmd = g_decoder.Decode((const char*)CMD_BUF);
    g_log.Printf(kLogInfo, "Decoded:%s", GetDeviceKeyString(cmd.output).c_str());
    switch (cmd.output) {
      case kCommandGetValueLedPinDevice:
        break;
      case kCommandGetValuePWM1Device:
        break;
      case kCommandSetValueLedPinDevice:
        if (cmd.value.stored_type == uComsValueInt) {
          if (cmd.value.value_int >= 0 && cmd.value.value_int <= 1) {
            g_log.Printf(kLogInfo, "setting LED to %d", cmd.value.value_int);
            gpio_put(LED_PIN, cmd.value.value_int);
          } else {
            g_log.Printf(kLogError, "arg:%d out of range", cmd.value.value_int);
          }
        }
        break;
      case kCommandSetValuePWM1Device:
        break;
      case kLenCommandsDevice:
        break;
      default:
        break;
    }
  }
  index = 0;
  memset((void*)CMD_BUF, '\0', CMD_LEN);
}

int main() {
  stdio_init_all();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  getchar();
  g_log.Printf(kLogInfo, "Welcome to Pico Enclosure!");
  // Flush any unused input
  while (true) {
    preParse(getchar_timeout_us(100));
  }
  return 0;
}
