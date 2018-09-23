#include "json.h"

string json_message(double x, double y, double angle) {
  // Get time
  struct timeval tp;
  gettimeofday(&tp, NULL);

  string json = "{";
  json += string(" \"type\":\"slam\"");
  json += string(",\"x\":") + to_string(x);
  json += string(",\"y\":") + to_string(y);
  json += string(",\"angle\":") + to_string(angle);
  json += string(",\"timestamp\":") + to_string(tp.tv_sec) +
          string(".") + to_string(tp.tv_usec);
  json += "}";

  return json;
}
