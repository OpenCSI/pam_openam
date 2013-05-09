#include <stdlib.h>
#include <assert.h>
#include <curl/curl.h>
#include <string.h>
#include "commons.h"

#define VALID_RESPONSE "boolean=true"

void init_response_data(struct response_data *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    assert(s->ptr != NULL);
    s->ptr[0] = '\0';
}

size_t write_response_data(void *ptr, size_t size, size_t nmemb, struct response_data *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  assert(s->ptr != NULL);
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

/*
 * Return 0 is the token is valid, 1 otherwhise
 */
int is_token_valid(struct response_data *s) {
    /* TODO: ensure s is longh enough*/
    int i;
    i = strncmp(s->ptr, VALID_RESPONSE, sizeof(VALID_RESPONSE) - 1);
    if (i == 0) {
        return i;
    } else {
        return 1;
    }
}