#ifndef __PROXY_H__
#define __PROXY_H__

#include "csapp.h"
#include "cache.h"

static const char *user_agent = "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Safari/537.36\r\n";
static const char *accept_h = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding = "Accept-Encoding: gzip, deflate\r\n";
static const char *connection = "Connection: close\r\n";

typedef struct {
    short valid;
    char  url[MAXLINE];
    char  hostname[MAXLINE];
    char  serverPort[6];
    char  path[MAXLINE];
    char  hostHeader[MAXLINE];
    char  otherHeaders[MAXLINE];
} Request;

//List proxy functions here...

#endif /* __PROXY_H__ */
