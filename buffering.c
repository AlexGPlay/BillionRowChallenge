#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 5*1024*1024
#define INITIAL_MEASUREMENTS 1500

#define DEBUG_LINES

struct NamedNode{
  char* name;
  float min;
  float max;
  float sum;
  int count;
};

struct NameLinkedList{
  char* name;
  struct NameLinkedList* next;
};

int hash(char* name, int size, int iterations){
  int hash = 0;
  for(int i = 0; i < strlen(name); i++){
    hash += name[i];
  }
  return (hash + iterations) % size;
}

int find(char* name, struct NamedNode *measurements, int size){
  for(int i = 0; i < size; i++){
    int index = hash(name, size, i);
    if(measurements[index].name == NULL){
      return -1;
    }

    if(strcmp(measurements[index].name, name) == 0){
      return index;
    }
  }

  return -1;
}

int getAvailablePosition(char* name, struct NamedNode *measurements, int size){
  for(int i = 0; i < size; i++){
    int index = hash(name, size, i);
    if(measurements[index].name == NULL){
      return index;
    }
  }

  return -1;
}

int add(char* name, int value, struct NamedNode* measurements, int size){
  struct NamedNode newNode;
  newNode.name = (char*)malloc(strlen(name) + 1 * sizeof(char));
  strcpy(newNode.name, name);
  newNode.min = value;
  newNode.max = value;
  newNode.sum = value;
  newNode.count = 1;

  int index = getAvailablePosition(name, measurements, size);
  measurements[index] = newNode;

  return index;  
}

int sortedAddToLinkedList(char* name, struct NameLinkedList** sortedMeasurementsNamesPtr){
  struct NameLinkedList* sortedMeasurementsNames = *sortedMeasurementsNamesPtr;

  if(sortedMeasurementsNames == NULL){
    struct NameLinkedList* newNode = (struct NameLinkedList*)malloc(sizeof(struct NameLinkedList));
    newNode->name = (char*)malloc(strlen(name) + 1 * sizeof(char));
    strcpy(newNode->name, name);
    newNode->next = NULL;

    *sortedMeasurementsNamesPtr = newNode;

    return 1;
  }

  struct NameLinkedList* newNode = (struct NameLinkedList*)malloc(sizeof(struct NameLinkedList));
  newNode->name = (char*)malloc(strlen(name) + 1 * sizeof(char));
  strcpy(newNode->name, name);
  newNode->next = NULL;

  if(strcmp(sortedMeasurementsNames->name, name) > 0){
    newNode->next = sortedMeasurementsNames;
    *sortedMeasurementsNamesPtr = newNode;

    return 1;
  }


  struct NameLinkedList* current = sortedMeasurementsNames;
  struct NameLinkedList* previous = NULL;
  while(current != NULL){
    if(strcmp(current->name, name) > 0){
      newNode->next = current;
      previous->next = newNode;

      return 1;
    }

    previous = current;
    current = current->next;
  }

  previous->next = newNode;

  return -1;
}

void addMeasurement(char* name, float value, struct NamedNode **measurementsPtr, int* currentSize, int* insertedSize, struct NameLinkedList **sortedMeasurementsNamesPtr){
  struct NamedNode *measurements = *measurementsPtr;

  int index = find(name, measurements, *currentSize);
  if(index == -1){
    index = add(name, value, measurements, *currentSize);

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


int main(){
  char buffer[BUFFER_SIZE];
  int oveflowRemainingBytes = 0;
  char *overflow = NULL;

  int currentSize = INITIAL_MEASUREMENTS;
  int insertedSize = 0;

  #ifdef DEBUG_LINES
  int lines = 0;
  #endif

  struct NamedNode *measurements;
  measurements = (struct NamedNode*)malloc(currentSize * sizeof(struct NamedNode));

  struct NameLinkedList *sortedMeasurementsNames = NULL;

  FILE *file = fopen("measurements.txt", "r");

  while (1) {
    size_t bytesRead = fread(buffer, 1, BUFFER_SIZE, file);
    size_t pendingBytes = bytesRead;

    if (bytesRead == 0) {
        break;
    }

    char* current;

    if (overflow != NULL) {
      char *newData = (char*)malloc(oveflowRemainingBytes + BUFFER_SIZE + 1);

      memcpy(newData, overflow, oveflowRemainingBytes);
      memcpy(newData + oveflowRemainingBytes, buffer, BUFFER_SIZE);

      free(overflow);
      current = newData;
      overflow = NULL;
      pendingBytes += oveflowRemainingBytes;
    }
    else{
      current = buffer;
    }


    char *nextLine;
    do {
      nextLine = strchr(current, '\n');
      if (nextLine != NULL) {
          *nextLine = '\0';

          pendingBytes -= strlen(current) + 1;

          char *name = strtok(current, ";");
          char *value = strtok(NULL, ";");

          float fvalue = atof(value);
          addMeasurement(name, fvalue, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);

          #ifdef DEBUG_LINES
          lines++;
          if(lines % 1000000 == 0){
            printf("Lines: %d\n", lines);
          }
          #endif

          current = nextLine + 1;
        } else {
          oveflowRemainingBytes = pendingBytes;
          overflow = malloc(oveflowRemainingBytes + 1);
          strncpy(overflow, current, oveflowRemainingBytes);
          break;
        }
    } while (nextLine != NULL);
  }


  fclose(file);

  struct NameLinkedList* current = sortedMeasurementsNames;
  while(current != NULL){
    int position = find(current->name, measurements, currentSize);
    printf("%s: %f %f %f\n", current->name, measurements[position].min, measurements[position].max, measurements[position].sum / measurements[position].count);
    current = current->next;
  }

  return 0;
}