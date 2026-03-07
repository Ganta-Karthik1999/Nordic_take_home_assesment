#ifndef JSON_H
#define JSON_H

#include "common.h"
#include <json-c/json.h>

/**
 * @brief Fuction that parses json file to read the rand value
 * 
 * **/
int parse_json_data(const char *json_str, uint64_t *rand_value);

#endif