#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define INITIAL_MEASUREMENTS 1500
#define NTHREADS 8
#define FILE_NAME "measurements.txt"

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

void processChunk(void* params){
  struct ThreadParams* parsedParams = (struct ThreadParams*)params;
  size_t from = parsedParams->from;
  size_t size = parsedParams->size;
  char *data = parsedParams->data;
  struct ReturnData* returnData = malloc(sizeof(struct ReturnData));

  int currentSize = INITIAL_MEASUREMENTS;
  int insertedSize = 0;

  struct NamedNode *measurements;
  measurements = (struct NamedNode*)malloc(currentSize * sizeof(struct NamedNode));

  struct NameLinkedList *sortedMeasurementsNames = NULL;

  char* line = data + from;

  char* nextLine = strchr(line, '\n');
  if(from == 0 || *(data + from - 1) == '\n'){
    size_t bytes = nextLine - line;
    char* lineCopy = (char*)malloc(bytes + 1);
    strncpy(lineCopy, line, bytes);
    lineCopy[bytes] = '\0';
    
    char *name = strtok(lineCopy, ";");

    char *value = strtok(NULL, ";");
    float fvalue = atof(value);

    addMeasurement(name, fvalue, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);
  }
  
  line = nextLine + 1;

  while(line < data + from + size){
    char *strtokPtr;

    nextLine = strchr(line, '\n');
    size_t bytes = nextLine - line;

    if(nextLine == NULL){
      bytes = data + from + size - line;
      char* lineCopy = (char*)malloc(bytes + 1);
      strncpy(lineCopy, line, bytes);
      lineCopy[bytes] = '\0';
      
      char *name = strtok_r(lineCopy, ";", &strtokPtr);

      char *value = strtok_r(NULL, ";", &strtokPtr);
      float fvalue = atof(value);

      addMeasurement(name, fvalue, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);
      break;
    }

    char* lineCopy = (char*)malloc(bytes + 1);
    strncpy(lineCopy, line, bytes);
    lineCopy[bytes] = '\0';
    
    char *name = strtok_r(lineCopy, ";", &strtokPtr);

    char *value = strtok_r(NULL, ";", &strtokPtr);
    float fvalue = atof(value);

    addMeasurement(name, fvalue, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);

    line = nextLine + 1;
  }

  returnData->measurements = measurements;
  returnData->sortedMeasurementsNames = sortedMeasurementsNames;

  parsedParams->returnData = returnData;

  pthread_exit(NULL);
}

void mergeResults(struct NamedNode **mergedMeasurementsPtr, struct NameLinkedList **mergedSortedListPtr, struct NamedNode **threadMeasurementsPtr, struct NameLinkedList **threadSortedListPtr){
  struct NamedNode *mergedMeasurements = *mergedMeasurementsPtr;
  struct NameLinkedList *mergedSortedList = *mergedSortedListPtr;
  struct NamedNode *threadMeasurements = *threadMeasurementsPtr;
  struct NameLinkedList *threadSortedList = *threadSortedListPtr;
  
  struct NameLinkedList *current = threadSortedList;
  while(current){
    int mergedIndex = find(current->name, mergedMeasurements, INITIAL_MEASUREMENTS);
    int threadIndex = find(current->name, threadMeasurements, INITIAL_MEASUREMENTS);

    struct NamedNode *threadNode = &threadMeasurements[threadIndex];

    if(mergedIndex == -1){
      sortedAddToLinkedList(current->name, mergedSortedListPtr);
      
      int availableIndex = getAvailablePosition(current->name, mergedMeasurements, INITIAL_MEASUREMENTS);
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

int main(){
  int fd;
  struct stat statbuf;

  fd = open(FILE_NAME, O_RDONLY);
  fstat(fd, &statbuf);

  pthread_t threads[NTHREADS];

  int chunkSize = statbuf.st_size / NTHREADS;
  size_t current = 0;

  struct ThreadParams params[NTHREADS];

  char* data = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);

  for(int i = 0; i < NTHREADS; i++){
    size_t end = current + chunkSize;

    params[i].id = i;
    params[i].from = current;
    params[i].size = chunkSize;
    params[i].data = *(&data);

    current = end;
    
    pthread_create(&threads[i], NULL, processChunk, &params[i]);
  }

  for (int i = 0; i < NTHREADS; i++){
    pthread_join(threads[i], NULL);
  }

  munmap(params[0].data, statbuf.st_size);
  close(fd);

  struct NameLinkedList *mergedMeasurementsNames = NULL;
  struct NamedNode *mergedMeasurements = (struct NamedNode*)malloc(INITIAL_MEASUREMENTS * sizeof(struct NamedNode));

  for(int i = 0; i < NTHREADS; i++){
    mergeResults(&mergedMeasurements, &mergedMeasurementsNames, &params[i].returnData->measurements, &params[i].returnData->sortedMeasurementsNames);
  }

  struct NameLinkedList *currentNode = mergedMeasurementsNames;
  while(currentNode){
    int index = find(currentNode->name, mergedMeasurements, INITIAL_MEASUREMENTS);

    printf("%s - Max: %f - Min: %f - Mean: %f\n", mergedMeasurements[index].name, mergedMeasurements[index].max, mergedMeasurements[index].min, (mergedMeasurements[index].sum / mergedMeasurements[index].count));
    currentNode = currentNode->next;
  }

  return 0;
}