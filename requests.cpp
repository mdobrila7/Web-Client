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

char *compute_get_request(string host, string url, string query_params,
                            string cookie)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    memset(line, 0, LINELEN);
    if (query_params != "") {
        sprintf(line, "GET %s?%s HTTP/1.1", url.c_str(), query_params.c_str());
    } else {
        sprintf(line, "GET %s HTTP/1.1", url.c_str());
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookie != "") {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: ");
        strcat(line, cookie.c_str());
        compute_message(message, line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(string host, string url, string content_type, string body_data, string cookie)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    memset(line, 0, LINELEN);
    sprintf(line, "POST %s HTTP/1.1", url.c_str());
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type.c_str());
    compute_message(message, line);

    if (body_data != "") {
        memset(body_data_buffer, 0, LINELEN);
        strcat(body_data_buffer, body_data.c_str());
    }

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);
    // Step 4 (optional): add cookies
    if (cookie != "") {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: ");
        strcat(line, cookie.c_str());
        compute_message(message, line);
    }
    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    if (body_data != "") {
        compute_message(message, body_data_buffer);
    }

    free(line);
    return message;
}

char *compute_get_authorization_request(string host, string url, string query_params,
                            string cookie)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    memset(line, 0, LINELEN);
    if (query_params != "") {
        sprintf(line, "GET %s?%s HTTP/1.1", url.c_str(), query_params.c_str());
    } else {
        sprintf(line, "GET %s HTTP/1.1", url.c_str());
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookie != "") {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer ");
        strcat(line, cookie.c_str());
        compute_message(message, line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_delete_authorization_request(string host, string url, string query_params,
                            string cookie)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    memset(line, 0, LINELEN);
    if (query_params != "") {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url.c_str(), query_params.c_str());
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url.c_str());
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookie != "") {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer ");
        strcat(line, cookie.c_str());
        compute_message(message, line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_authorization_request(string host, string url, string content_type, string body_data, string cookie)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    memset(line, 0, LINELEN);
    sprintf(line, "POST %s HTTP/1.1", url.c_str());
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type.c_str());
    compute_message(message, line);

    if (body_data != "") {
        memset(body_data_buffer, 0, LINELEN);
        strcat(body_data_buffer, body_data.c_str());
    }

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);
    // Step 4 (optional): add cookies
    if (cookie != "") {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer ");
        strcat(line, cookie.c_str());
        compute_message(message, line);
    }
    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    if (body_data != "") {
        compute_message(message, body_data_buffer);
    }

    free(line);
    return message;
}
