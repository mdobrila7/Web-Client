#ifndef _REQUESTS_
#define _REQUESTS_

#include <string>

using namespace std;

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(string host, string url, string query_params,
                            string cookie);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(string host, string url, string content_type, string body_data, string cookie);

char *compute_get_authorization_request(string host, string url, string query_params,
                            string cookie);

char *compute_delete_authorization_request(string host, string url, string query_params,
                            string cookie);

char *compute_post_authorization_request(string host, string url, string content_type, string body_data, string cookie);

#endif
