#include "csapp.h"
#include "cache.h"

int is_in_cache(char* uri) {

}

int save_in_cache(CachedItem item) {
  if(list->size > 0) {
    item.prev = list->last;
    *(list->last)->next = item;
    list->last = &item;
  } else {
    list->last = &item;
    list->first = &item;
  }

  // Always increment size by 1
  list->size = list->size + 1;
  printf(*(list->last)->url);
  return 1;
}

CachedItem get_from_cache() {

}