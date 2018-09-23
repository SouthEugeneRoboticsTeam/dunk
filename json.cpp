#include "json.h"

string json_message(double x, double y, double angle) {
  // Get time
  struct timeval tp;
  gettimeofday(&tp, NULL);
  long int epoch_s = tp.tv_sec;
  long int epoch_ms = tp.tv_usec * 1000;  
  string json = "{";
  json += string(" \"epoch_s\": ") + to_string(epoch_s);
  json += string(",\"epoch_ms\": ") + to_string(epoch_ms);
  json += string(",\"type\":\"slam\"");
  json += string(",\"x\":") + to_string(x);
  json += string(",\"y\":") + to_string(y);
  json += string(",\"angle\":") + to_string(angle);
  json += "}";
  return json;
}
