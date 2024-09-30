#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "handlers.h"

int nodeCounter; // check how many nodes we have ca 340 max)

struct LinkedList *createNode(unsigned int id, float sensorData)
{
  struct LinkedList *p;
  p = (struct LinkedList *)malloc(sizeof(struct LinkedList)); // Allocate memory

  if (p == NULL)
  {
    Flag2 |= (1 << 0);
    return NULL;
  }
  p->id = id;
  p->sensorData = sensorData;

  nodeCounter++;
  p->next = NULL;
  return p; // return pointer
}

void insertFirst(struct LinkedList **first, struct LinkedList *el)
{
  if (el == NULL) // If el == NULL return
  {
    //   printf("Cannot insert NULL value. (insert first) \n");
    return;
  }

  if (first == NULL)
  {
    //  printf("List cannot be NULL. (insert first)\n");
    return;
  }

  el->next = *first; // insert first in the element after the new.
  *first = el;
  return;
}

void freeList(struct LinkedList *first)
{
  struct LinkedList *temp, *ptr;
  ptr = first;

  while (ptr->next != NULL)
  {
    temp = ptr;
    ptr = ptr->next;
  }
  temp->next = NULL;
  free(ptr);

  nodeCounter--;
}

struct LinkedList *readSensor(int id)
{
  struct LinkedList *tmp;

  if (id == NULL)
  {
    printf("Cannot add NULL node. (readSensor)\n");
    return NULL;
  }
  float sensData = tempVal;
  tmp = createNode(id, sensData);
  tmp->next = NULL;
  return tmp; // return pointer
}