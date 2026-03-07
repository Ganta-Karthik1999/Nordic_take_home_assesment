#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdint.h>
#include <json-c/json.h>

int parse_json_data(const char *json_str, uint64_t *rand_value);

#endif