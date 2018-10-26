#include "csapp.h"
#include "cache.h"

int save_in_cache(struct CachedItem *item)
{

  struct CachedItem *new = (struct CachedItem *)malloc(sizeof(struct CachedItem));
  strcpy(new->url, item->url);
  new->data = item->data;
  new->size = sizeof(item->data);

  if (list.size == 0)
  { // Empty list - just started filling
    // Should add the current item as the first item
    // First -> new item
    list.first = new;
    list.last = new;
    new->next = NULL;
    new->prev = NULL;
  }
  else
  {
    // item.prev = list.first; // Old first prev -> new item
    list.last->next = new;
    new->prev = list.last;
    new->next = NULL;
    list.last = new;
  }

  ++list.size;

  printf("List first: %s\n", list.first->url);
  printf("List Before last: %s\n", list.last->prev->url);
  printf("List last: %s\n", list.last->url);
  // printf("list.size: %d\n", (int)list.size);
  // printf("FIRST: %s\n", list.first->url);
  // printf("NEW FIRST1111: %s\n", list.first->next->url);
  return 1;
}

CachedItem* get_from_cache(char* uri) {
  if (list.size == 0)
  {
    return NULL;
  }
  struct CachedItem *ptr = list.first;
  printf("cache check, %s\n", uri);
  int i = 0;
  while (ptr->next != NULL)
  {
    if (strcmp(ptr->url, uri) == 0)
    {
      printf("found!!!!!!, %d\n", i);
      return ptr;
    }
    ptr = ptr->next;
    i++;
  }
  return NULL;
}