#ifndef __CACHE_H__
#define __CACHE_H__

#include "csapp.h"

#define MAX_CACHE_SIZE 104857600  //100 Mb
#define MAX_OBJECT_SIZE 819200    //100 Kb

typedef struct CachedItem CachedItem;

struct CachedItem {
  char url[MAXLINE];
  void *data;
  size_t size;
  struct CachedItem *prev;
  struct CachedItem *next;
};

typedef struct {
  size_t size;
  struct CachedItem* first;
  struct CachedItem* last;
} CacheList;

int save_in_cache(struct CachedItem item);
CachedItem* get_from_cache(char* uri);

CacheList list;

#endif /* __CACHE_H__ */
