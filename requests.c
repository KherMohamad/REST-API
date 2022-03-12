#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url,
                            char *cookie, char *JWT)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    //if (query_params != NULL) {
      //  sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    //} else {
        sprintf(line, "GET %s HTTP/1.1", url);
    

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    if (JWT != NULL) {
       compute_message(message, JWT);
    }
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookie != NULL) {
        compute_message(message, cookie);
        
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}
char *compute_delete_request(char *host, char *url,
                            char *cookie, char *JWT)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    //if (query_params != NULL) {
      //  sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    //} else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    if (JWT != NULL) {
       compute_message(message, JWT);
    }
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookie != NULL) {
        compute_message(message, cookie);
        
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}
char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                             char *cookie, char *JWT)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s  HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    sprintf(line, "Content-Type: %s", content_type);
    //compute_message(message, "Content-Type: application/json");
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);
    if (JWT != NULL) {
       compute_message(message, JWT);
    }
    
     //Step 4 (optional): add cookies
    if (cookie != NULL) {
        compute_message(message, cookie);
        
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    //memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}
