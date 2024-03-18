#pragma once

#include <stdio.h>
#include "./data.h"

struct ReturnData{
  struct NamedNode *measurements;
  struct NameLinkedList *sortedMeasurementsNames;
};

struct ThreadParams{
  int id;
  size_t from;
  size_t size;
  char* data;
  struct ReturnData* returnData;
};

void mergeResults(struct NamedNode **mergedMeasurementsPtr, struct NameLinkedList **mergedSortedListPtr, struct NamedNode **threadMeasurementsPtr, struct NameLinkedList **threadSortedListPtr, int hashSize);