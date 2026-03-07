#include"post.h"


/**
 * @brief Function callback that captures responses
 * @param ptr Pointer to the data received from the HTTP response
 * @param size Size of each data element
 * @param nmemb Number of data elements received
 * @param response_buffer Pointer to the destination buffer where data will be appended
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
 * @return void
 */
void send_post_request(uint64_t odd, uint64_t even) {
    CURL *curl;
    CURLcode res;
    char post_data[128];
    char response[2048] = {0};

    // Format: "odd=42&even=22"
    snprintf(post_data, sizeof(post_data), "odd=%lu even=%lu", odd, even);

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
