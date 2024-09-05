#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host,const char *url, char *query_params,
							std::vector<char *>cookies, int cookies_count,const  char *tok);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, const char *url,const char* content_type, std::string jsonStr, char **cookies, int cookies_count,const char *token);

char *compute_delete_book(char *host, const char *url, const char* token);

#endif
