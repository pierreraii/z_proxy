#include "csapp.h"
#include "cache.h"

int is_in_cache(char* uri) {

}

int save_in_cache(CachedItem item) {
  if(list.size == 0){
    list.first = &item;
  } else{
    list.last->next = &item;
    item.prev = list.last;
  }
  list.last = &item;
  ++list.size;
  printf("list.size: %s\n", list.last->url);
  return 1;
}

CachedItem get_from_cache() {

}