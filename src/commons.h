/* 
 * File:   commons.h
 * Author: asyd
 *
 * Created on 9 mai 2013, 12:50
 */

#ifndef COMMONS_H
#define	COMMONS_H

struct response_data {
  char *ptr;
  size_t len;
};

size_t write_response_data(void *ptr, size_t size, size_t nmemb, struct response_data *s);
void init_response_data(struct response_data *s);

int is_token_valid(struct response_data *s);

#endif	/* COMMONS_H */

