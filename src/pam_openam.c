#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define OPENAM_URL "https://sso.opencsi.com/openam"

int
main(int argc, char* argv[]) {
    
    if (argc < 2) {
        printf("Usage: %s token\n", argv[0]);
        exit (1);
    }
    CURL *curl;
    CURLcode res;
    char *realm = "/employees";
    char *username = "bbonfils";
//    char *token = "AQIC5wM2LY4Sfcz6-iezPIDQmx7PNg7vPxa9j_ff6FSuObc.*AAJTSQACMDEAAlNLABQtMzc4Mjk3OTI0NDc1MDk4OTc2Ng..*";
    char *token = argv[1];

    char *url;
    
    asprintf(&url, "%s/identity/isTokenValid?tokenid=%s", OPENAM_URL, token);
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        
        if (res == CURLE_OK) {
            printf("OK\n");
        } else {
            printf("curl_easy_perform failed: %s\n", curl_easy_strerror(res));
        }
    }
    printf("Hello world %s\n%s\n", username, url);
    
    curl_easy_cleanup(curl);
}
