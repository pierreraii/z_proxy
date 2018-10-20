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
  CachedItem *prev;
  CachedItem *next;
};

typedef struct {
  size_t size;
  CachedItem* first;
  CachedItem* last;
} CacheList;

//List cache functions here...

#endif /* __CACHE_H__ */
