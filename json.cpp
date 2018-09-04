#include "json.h"

string build_json_msg(double x, double y, double angle) {
    string tmp = "{";
    tmp += string( "\"x\":") + to_string(x);
    tmp += string(",\"y\":") + to_string(y);
    tmp += string(",\"angle\":") + to_string(angle);
    tmp += "}";
    return tmp;
}
