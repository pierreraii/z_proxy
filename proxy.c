#include "csapp.h"
#include "cache.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "string.h"
#include "pthread.h"
#define PORT 80

void proxy(void *vargp);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void parse_request_headers(rio_t *rp, char *hostname, char *path, char *proxy_buffer);

/* Thread lock */
pthread_rwlock_t lock;

/* Mutex */
static sem_t mutex;

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;
    pthread_t tid;
    pthread_rwlock_init(&lock, 0);
    // Sem_init(&mutex, 0, 1);

    /* Check command line args */
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        /* Create thread and let thread handle functions */
        if ((connfd > 0))
        {
            Pthread_create(&tid, NULL, proxy, &connfd);
        }
    }
}

/*
 * proxy - handle one HTTP request/response transaction
 */
void proxy(void *vargp)
{
    int serverfd;
    int content_len;
    int clientfd = *((int *)vargp);

    char client_buffer[MAXLINE];
    char server_buffer[MAXLINE];
    char proxy_buffer[MAXLINE] = "";

    char *port = "80";
    char uri[MAXLINE];
    char path[MAXLINE];
    char method[MAXLINE];
    char version[MAXLINE];
    char hostname[MAXLINE];

    rio_t client_rio;
    rio_t server_rio;

    pthread_detach(pthread_self());
    // Free(vargp);
    /** Receive Request from Client */
    Rio_readinitb(&client_rio, clientfd);

    if (!Rio_readlineb(&client_rio, client_buffer, MAXLINE))
    {
        return;
    }

    /** Parse Method URI Version from Headers */
    sscanf(client_buffer, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "GET"))
    {
        clienterror(clientfd, method, "501", "Not Implemented", "Tiny does not implement this method");
        return;
    }

    /** Parse SubPath from Headers */
    sscanf(uri, "http://%*[^/]%s", path);

    /** Parse Hostname from Headers */
    parse_request_headers(&client_rio, hostname, path, proxy_buffer);

    // printf("\n\nPROXYBUFFER %s\n\n", proxy_buffer);

    if ((serverfd = Open_clientfd(hostname, port)) == -1)
    {
        clienterror(clientfd, path, "404", "Not found", "Tiny couldn't find this file");
        return;
    }

    rio_readinitb(&server_rio, serverfd);

    if (rio_writen(serverfd, proxy_buffer, strlen(proxy_buffer)) < 0)
    {
        clienterror(clientfd, method, "500", "Server Error", "Tiny encountered an unexpected error");
        close(serverfd);
        return;
    }

    char *response[MAXLINE];
    while ((content_len = rio_readnb(&server_rio, server_buffer, MAXLINE)) > 0)
    {
        strcat(response, server_buffer);
        rio_writen(clientfd, server_buffer, content_len);
    }

    struct CachedItem item;
    item.size = sizeof(response);
    item.data = response;
    strcpy(item.url, uri);

    save_in_cache(item);
    Close(clientfd);
}

/*
 * parse_request_headers - read HTTP request headers
 */
void parse_request_headers(rio_t *rp, char *hostname, char *path, char *proxy_buffer)
{
    char buffer[MAXLINE];

    Rio_readlineb(rp, buffer, MAXLINE);

    // Get Hostname from
    sscanf(buffer, "Host: %s", hostname);

    /** Build new proxy request headers */
    strcat(proxy_buffer, "GET http://");
    strcat(proxy_buffer, hostname); // hostname example: lau.edu.lb
    strcat(proxy_buffer, path);     // path example: /index.aspx ;-)
    strcat(proxy_buffer, " HTTP/1.0\r\n"); // override http version
    strcat(proxy_buffer, buffer);

    while (strcmp(buffer, "\r\n"))
    {
        Rio_readlineb(rp, buffer, MAXLINE);

        if(strstr(buffer, "Proxy-Connection: ")){
            strcpy(buffer, "Proxy-Connection: close\r\n");
        } else if(strstr(buffer, "Connection:")){
            strcpy(buffer, "Connection: close\r\n");
        }
        
        if(strstr(buffer, "Keep-Alive:")){
            strcpy(buffer, "");
        }

        strcat(proxy_buffer, buffer);
    }

    strcat(proxy_buffer, "\r\n");
    printf("%s", proxy_buffer);
    return;
}

/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor="
                  "ffffff"
                  ">\r\n",
            body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}