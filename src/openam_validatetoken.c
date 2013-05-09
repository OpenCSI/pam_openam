#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <assert.h>

#include "commons.h"

#define OPENAM_URL "https://sso.opencsi.com/openam"

int
main(int argc, char* argv[]) {
    int retcode = 1;
    
    /* Usage */
    if (argc < 3) {
        printf("Usage: %s username token\n", argv[0]);
        exit(1);
    }

    /* Initialize CURL */
    CURL *curl = NULL;
    CURLcode res;

    /* Variables */
    char *username = argv[1];
    char *token = argv[2];

    char *url;

    asprintf(&url, "%s/identity/isTokenValid?tokenid=%s", OPENAM_URL, token);

    assert(url != NULL);

    curl = curl_easy_init();

    assert(curl != NULL);

    
    /* Response */
    struct response_data data;
    init_response_data(&data);

    /* Set CURL options */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    
    /* Perform GET */
    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        if(is_token_valid(&data) == 0) {
            printf("Token is valid\n");
            retcode = 0;
        } else {            
            printf("Token is invalid\n");
        }
    } else {
        printf("curl_easy_perform failed: %s\n", curl_easy_strerror(res));
    }

    /* Cleanup memory */
    free(url);
    free(data.ptr);
    curl_easy_cleanup(curl);
    
    return retcode;
}
