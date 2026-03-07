#ifndef JSON_H
#define JSON_H

#include "common.h"
#include <json-c/json.h>

int parse_json_data(const char *json_str, uint64_t *rand_value);

#endif