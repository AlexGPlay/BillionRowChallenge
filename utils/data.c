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

int parseFloat(char* number, float* value){
  int mod = 1;
  if(number[0] == '-'){
    mod = -1;
    number++;
  }

  if(number[1] == '.'){
    *value = ((number[0] - '0') + (number[2] - '0') / 10.0) * mod;
    return 4;
  }

  *value = (((number[0] - '0') * 10) + (number[1] - '0') + (number[3] - '0') / 10.0) * mod;
  return 5;
}

struct StationData parseLine(char* line){
  struct StationData data;

  int i = 0;
  while(line[i] != ';'){
    i++;
  }

  char* name = malloc(i + 1);
  strncpy(name, line, i);
  name[i] = '\0';

  float temperature;
  data.nextLine = line + i + parseFloat(line + i + 1, &temperature);
  data.name = name;
  data.temperature = temperature;
  data.processedBytes = data.nextLine - line;

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