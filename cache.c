#include "csapp.h"
#include "cache.h"

int save_in_cache(struct CachedItem item) {
  struct CachedItem default_item;
  default_item.size = 0;
  default_item.data = "n/a";
  strcpy(default_item.url, "n/a");

  item.prev = &default_item;

  if(list.size == 0){
    item.next = &default_item;
    list.first = &item;
  } else{
    item.prev = list.first;
    list.first = &item;
  }

  ++list.size;
  
  printf("list.size: %d\n", (int)list.size);
  printf("FIRST: %s\n", list.first->url);
  return 1;
}

CachedItem* get_from_cache(char* uri) {
  if(list.size == 0) {
    return NULL;
  }

  char na_url[MAXLINE];
  struct CachedItem *current_item = list.first;

  strcpy(na_url, "n/a");

  while(!strcmp(current_item->url, na_url)) {
    printf("URI %s\n", uri);
    printf("URLRLRLRL %s\n", current_item->url);
    if(strcmp(current_item->url, uri) == 0) {
      return current_item;
    }

    current_item = current_item->next;
  }

  return NULL;
}