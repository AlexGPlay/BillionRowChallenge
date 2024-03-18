#include "./multithreading.h"
#include "./data.h"
#include "./hash.h"

void mergeResults(struct NamedNode **mergedMeasurementsPtr, struct NameLinkedList **mergedSortedListPtr, struct NamedNode **threadMeasurementsPtr, struct NameLinkedList **threadSortedListPtr, int hashSize){
  struct NamedNode *mergedMeasurements = *mergedMeasurementsPtr;
  struct NameLinkedList *mergedSortedList = *mergedSortedListPtr;
  struct NamedNode *threadMeasurements = *threadMeasurementsPtr;
  struct NameLinkedList *threadSortedList = *threadSortedListPtr;
  
  struct NameLinkedList *current = threadSortedList;
  while(current){
    int mergedIndex = findPositionInHash(current->name, mergedMeasurements, hashSize);
    int threadIndex = findPositionInHash(current->name, threadMeasurements, hashSize);

    struct NamedNode *threadNode = &threadMeasurements[threadIndex];

    if(mergedIndex == -1){
      sortedAddToLinkedList(current->name, mergedSortedListPtr);
      
      int availableIndex = getAvailablePosition(current->name, mergedMeasurements, hashSize);
      mergedMeasurements[availableIndex] = *threadNode;

      current = current->next;

      continue;
    }
    
    struct NamedNode *mergedNode = &mergedMeasurements[mergedIndex];

    if(threadNode->min < mergedNode->min){
      mergedNode->min = threadNode->min;
    }
    if(threadNode->max > mergedNode->max){
      mergedNode->max = threadNode->max;
    }
    mergedNode->sum += threadNode->sum;
    mergedNode->count += threadNode->count;

    current = current->next;
  }
}