#include"json.h"

/**
 * @brief Fuction that parses json file to read the rand value
 * @param json_str Pointer to a JSON string to parse
 * @param rand_value A uint64_t address to store the random value
 * @return Returns 0 on successful parsing, -1 if parsing fails
 */
int parse_json_data(const char *json_str,uint64_t *rand_value) {
    struct json_object *parsed_json;
    struct json_object *rand_value_obj;

    parsed_json = json_tokener_parse(json_str);
    if (parsed_json == NULL) {
        fprintf(stderr, "Error: Failed to parse JSON\n");
        return -1;
    }  

    if (json_object_object_get_ex(parsed_json, "rand_value", &rand_value_obj)) {
        *rand_value = json_object_get_uint64(rand_value_obj);
    } else {
        fprintf(stderr, "Error: 'rand_value' field not found\n");
        json_object_put(parsed_json);
        return -1;
    }

    // Free the JSON object
    json_object_put(parsed_json);
    return 0;
}
