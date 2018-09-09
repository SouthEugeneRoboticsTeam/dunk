#include "json.h"

string json_message(double x, double y, double angle) {
  string json = "{";
  json += string( "\"x\":") + to_string(x);
  json += string(",\"y\":") + to_string(y);
  json += string(",\"angle\":") + to_string(angle);
  json += "}";
  return json;
}
