#include "utils.h"
#include <curl/curl.h>
int use_request(char *url, char *header, char *data, char *socket_path, char *method) {
    // Init curl
    log(__func__, "requesting to %s", url);
    CURL *curl;
    CURLcode res;
    long http_code = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    // Set curl options
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, header);
        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, socket_path);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
       if (data != NULL && strcmp(method, "POST") == 0) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        }
        if (strcmp(method, "DELETE") == 0) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }
        if (data == NULL && strcmp(method, "POST") == 0) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        }
        // Perform curl - send request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return 1;
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        }
        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        fprintf(stderr, "curl_easy_init() failed\n");
        return 1;
    }
    curl_global_cleanup();
    return (int)http_code;
}