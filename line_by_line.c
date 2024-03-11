#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 50
#define INITIAL_MEASUREMENTS 10
#define DEBUG

struct NamedNode{
  char* name;
  float min;
  float max;
  float sum;
  int count;
};

void debugNode(struct NamedNode node){
  #ifdef DEBUG
  printf("DEBUG: Node: %s, min: %f, max: %f, sum: %f, count: %d\n", node.name, node.min, node.max, node.sum, node.count);
  #endif
}

int addMeasurement(char* name, float value, struct NamedNode *measurements, int currentSize){
  #ifdef DEBUG
  printf("DEBUG: Adding measurement (%s - %f) ; Current size: (%d)\n", name, value, currentSize);
  #endif

  for(int i = 0; i < currentSize; i++){
    char* currentName = measurements[i].name;

    if(currentName == NULL){
      #ifdef DEBUG
      printf("DEBUG: Found empty slot\n");
      #endif

      measurements[i].name = strdup(name);
      measurements[i].min = value;
      measurements[i].max = value;
      measurements[i].sum = value;
      measurements[i].count = 1;
      return currentSize;
    }

    #ifdef DEBUG
    printf("DEBUG: Comparing %s with %s\n", currentName, name);
    #endif

    if(strcmp(currentName, name) == 0){
      #ifdef DEBUG
      printf("DEBUG: Found existing measurement\n");
      #endif

      if(value < measurements[i].min){
        measurements[i].min = value;
      }
      if(value > measurements[i].max){
        measurements[i].max = value;
      }
      measurements[i].sum += value;
      measurements[i].count++;
      return currentSize;
    }
  }

  #ifdef DEBUG
  printf("ERROR: No space for new measurement, expanding array\n");
  #endif

  struct NamedNode* newMeasurements = malloc(sizeof(struct NamedNode) * (currentSize + 10));
  for(int i = 0; i < currentSize; i++){
    newMeasurements[i] = measurements[i];
  }
  free(measurements);
  measurements = newMeasurements;
  addMeasurement(name, value, measurements, currentSize + 10);
  return currentSize + 10;
}

int main(){
  #ifdef DEBUG
  printf("DEBUG: Starting program\n");
  #endif

  int currentSize = INITIAL_MEASUREMENTS;
  struct NamedNode measurements[INITIAL_MEASUREMENTS];
  #ifdef DEBUG
  printf("DEBUG: Created array\n");
  #endif

  FILE* file = fopen("measurements_min.txt", "r");
  #ifdef DEBUG
  printf("DEBUG: Opened file\n");
  #endif

  char line[LINE_SIZE];

  while(fgets(line, LINE_SIZE, file)){
    char* name = strtok(line, ";");
    char* value = strtok(NULL, ";");

    float fvalue = atof(value);
    currentSize = addMeasurement(name, fvalue, measurements, currentSize);
  }

  fclose(file);
  

  #ifdef DEBUG
  printf("DEBUG: Finished adding size at %d\n", currentSize);
  #endif

  for(int i = 0; i < currentSize; i++){
    if(measurements[i].name == NULL){
      break;
    }

    printf("%s: min: %f, max: %f, avg: %f\n", measurements[i].name, measurements[i].min, measurements[i].max, measurements[i].sum / measurements[i].count);
  }
  return 0;
}