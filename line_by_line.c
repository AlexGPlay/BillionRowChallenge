#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils/data.c"

#define LINE_SIZE 50
#define INITIAL_MEASUREMENTS 1500
#define DEBUG

void debugNode(struct NamedNode node){
  #ifdef DEBUG
  printf("DEBUG: Node: %s, min: %f, max: %f, sum: %f, count: %d\n", node.name, node.min, node.max, node.sum, node.count);
  #endif
}

int main(){
  #ifdef DEBUG
  printf("DEBUG: Starting program\n");
  #endif

  int size = INITIAL_MEASUREMENTS;
  int insertedSize = 0;

  struct NamedNode *measurements = (struct NamedNode*)malloc(size * sizeof(struct NamedNode));
  struct NameLinkedList *sortedMeasurementsNames = NULL;

  #ifdef DEBUG
  printf("DEBUG: Created data structures\n");
  #endif

  FILE* file = fopen("measurements.txt", "r");
  #ifdef DEBUG
  printf("DEBUG: Opened file\n");
  #endif

  char line[LINE_SIZE];

  while(fgets(line, LINE_SIZE, file)){
    char* name = strtok(line, ";");
    char* value = strtok(NULL, ";");

    float fvalue = atof(value);
    addMeasurement(name, fvalue, &measurements, &size, &insertedSize, &sortedMeasurementsNames);
  }

  fclose(file);
  

  #ifdef DEBUG
  printf("DEBUG: Finished adding size at %d\n", size);
  #endif

  printMeasurements(sortedMeasurementsNames, measurements, size);
  return 0;
}