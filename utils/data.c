#include <stdio.h>
#include <string.h>

#include "./data.h"
#include "./hash.c"
#include "./linkedlist.c"

void addMeasurement(char* name, float value, struct NamedNode **measurementsPtr, int* currentSize, int* insertedSize, struct NameLinkedList **sortedMeasurementsNamesPtr){
  struct NamedNode *measurements = *measurementsPtr;

  int index = findPositionInHash(name, measurements, *currentSize);
  if(index == -1){
    index = addToHash(name, value, measurements, *currentSize);

    sortedAddToLinkedList(name, sortedMeasurementsNamesPtr);

    (*insertedSize)++;
  } else {
    if(value < measurements[index].min){
      measurements[index].min = value;
    }

    if(value > measurements[index].max){
      measurements[index].max = value;
    }

    measurements[index].sum += value;
    measurements[index].count++;
  }
}

struct StationData parseLine(char* line){
  struct StationData data;

  char* nextLine = strchr(line, '\n');
  size_t bytes = 0;
  if(nextLine){
    bytes = nextLine - line;
  } else {
    bytes = strlen(line);
  }


  char* lineCopy = (char*)malloc(bytes + 1);
  strncpy(lineCopy, line, bytes);
  lineCopy[bytes] = '\0';

  char *strtokPtr;
  char *name = strtok_r(lineCopy, ";", &strtokPtr);

  char *value = strtok_r(NULL, ";", &strtokPtr);
  float fvalue = atof(value);

  data.name = name;
  data.temperature = fvalue;
  data.nextLine = nextLine;
  data.processedBytes = bytes;

  return data;
}

void printMeasurements(struct NameLinkedList *names, struct NamedNode *measurements, int measurementsSize){
  struct NameLinkedList *currentNode = names;
  while(currentNode){
    int index = findPositionInHash(currentNode->name, measurements, measurementsSize);

    printf("%s - Max: %f - Min: %f - Mean: %f\n", measurements[index].name, measurements[index].max, measurements[index].min, (measurements[index].sum / measurements[index].count));
    currentNode = currentNode->next;
  }
}