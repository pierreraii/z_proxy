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

  return 1;
}

CachedItem* get_from_cache(char* uri) {
  if (list.size == 0)
  {
    return NULL;
  }
  struct CachedItem *ptr = list.first;
  int i = 0;
  while (ptr->next != NULL)
  {
    if (strcmp(ptr->url, uri) == 0)
    {
      return ptr;
    }
    ptr = ptr->next;
    i++;
  }
  return NULL;
}