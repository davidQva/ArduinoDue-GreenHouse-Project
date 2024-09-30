#ifndef _linkedList_h
#define _linkedList_h

#include "linkedList.c"
#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

void insertFirst(struct LinkedList **first, struct LinkedList *el);
void freeList(struct LinkedList *first);

struct LinkedList *readSensor();
struct LinkedList *createNode(unsigned int id, float sensorData);

extern int nodeCounter;


#endif