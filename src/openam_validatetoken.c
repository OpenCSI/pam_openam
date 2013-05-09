#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <assert.h>

#include "commons.h"

int
main(int argc, char* argv[]) {
    int retcode = 1;
    
    /* Usage */
    if (argc < 4) {
        printf("Usage: %s openam_url username token\n", argv[0]);
        exit(1);
    }

    retcode = validate_token(argv[1], argv[2], argv[3]);
    if(retcode == 0) {
        printf("OK\n");
    } else {
        printf("KO\n");
    }
        
    return retcode;
}
