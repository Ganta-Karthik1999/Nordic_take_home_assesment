#include "socket_utils.h"


/**
 * @brief Singal handler for SIGINT(Interrupt Signal)
 * @param sig The signal number (SIGINT)
 * 
 * @return void
 */
void handle_sigint(int sig) {
    running = 0;
}


/**
 * @brief Fuction that parses json file to read the rand value
 * @param json_str Pointer to a JSON string to parse
 * @param rand_value A uint64_t address to store the random value
 * 
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


/**
 * @brief Function uses system clock to return time in milliseconds
 * 
 * @return Time in milliseconds
 */
long get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}


/**
 * @brief Function callback that captures responses
 * @param ptr Pointer to the data received from the HTTP response
 * @param size Size of each data element
 * @param nmemb Number of data elements received
 * @param response_buffer Pointer to the destination buffer where data will be appended
 * 
 * @return Total number of bytes processed
 */
size_t write_callback(char *ptr, size_t size, size_t nmemb, char *response_buffer) {
    size_t total_size = size * nmemb;
    strncat(response_buffer, ptr, total_size);
    return total_size;
}


/**
 * @brief Function to send POST request to paste.c-net.org
 * @param odd The number of odd values in the past 60 second window
 * @param odd The number of even values in the past 60 second window
 * 
 * @return void
 */
void send_post_request(uint64_t odd, uint64_t even) {
    CURL *curl;
    CURLcode res;
    char post_data[128];
    char response[2048] = {0};

    // Format: "odd=42&even=22"
    snprintf(post_data, sizeof(post_data), "odd=%lu&even=%lu", odd, even);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://paste.c-net.org/");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "POST failed: %s\n", curl_easy_strerror(res));
    } else {
        printf("Response: %s\n", response);
    }

    curl_easy_cleanup(curl);
}


// Thread function to handle each client
void *handle_client(void *arg) {
    int clientfd = *((int *)arg);
    free(arg);  
    char text[255];
    uint64_t rand_value;
    int status;

    memset(text, 0, sizeof(text));
    status = recv(clientfd, text, sizeof(text) - 1, 0);
    if (status > 0) {
        if (parse_json_data(text, &rand_value) == 0) {
            check_odd_or_even(rand_value);
        }
    }
    close(clientfd);
    pthread_mutex_lock(&count_mutex);
    active_connections--;
    printf("Client disconnected. Active connections: %d\n", active_connections);
    pthread_mutex_unlock(&count_mutex);
    return NULL;
}


void check_odd_or_even(uint64_t rand_value) {
    pthread_mutex_lock(&count_mutex);
    if (rand_value % 2 == 0) {
        even = even + 1;
    } else {
        odd = odd + 1;
    }
    pthread_mutex_unlock(&count_mutex);
}

