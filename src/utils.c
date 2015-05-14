#include <pebble.h>
#include "utils.h"

// Format uptime to 00:00:00
void format_uptime(char **format, int hours, int minutes, int seconds) {
  strcpy(*format,  "");
  
  if (hours < 10) {
    strcat(*format, "0%d:");
  } else {
    strcat(*format, "%d:");
  }
  if (minutes < 10) {
    strcat(*format, "0%d:");
  } else {
    strcat(*format, "%d:");
  }
  if (seconds < 10) {
    strcat(*format, "0%d");
  } else {
    strcat(*format, "%d");
  }
}