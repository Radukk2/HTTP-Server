#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.hpp"

char *compute_get_request(char *host, const char *url, char *query_params,
                            std::vector<char *>cookies, int cookies_count, const char *token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    // Step 1: write the method name, URL, request params (if any) and protocol type
    sprintf(line, "GET %s HTTP/1.1", url);

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    sprintf(line, "Cookie: %s", cookies[0]);
    // strcpy(message, cookies[0]);
    // Step 4: add final new line
    compute_message(message, line);
    if (token) {
        sprintf(line, "Authorization: Bearer: %s", token);
        compute_message(message, line);
    }
    strcat(message, "\r\n");
    free(line);
    return message;
}

char *compute_post_request(char *host,const char *url, const char* content_type, std::string jsonStr, char **cookies, int cookies_count,const char *token) {
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));
    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */

	sprintf(line, "Content-Type: %s", content_type);
	compute_message(message, line);
	int len;
    len = jsonStr.size();
	sprintf(line, "Content-Length: %d", len);
	compute_message(message, line);

    if (token) {
        sprintf(line, "Authorization: Bearer: %s", token);
        compute_message(message, line);
    }
    // line = "Connection: Keep-Alive";
    // compute_message(message, line);
	strcat(message, "\r\n");
    strcat(message, jsonStr.c_str());

    // Step 4 (optional): add cookies
    if (cookies != NULL) {
       
    }
    strcat(message, "\r\n");
    // Step 5: add new line at end of header

    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

	// printf("sending: %s\n", message);

    free(line);
    return message;
}

char *compute_delete_book(char *host, const char *url, const char *token) {
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
	compute_message(message, line);
    if (token) {
        sprintf(line, "Authorization: Bearer: %s", token);
        compute_message(message, line);
    }
    strcat(message, "\r\n");
    free(line);
    return message;
}
