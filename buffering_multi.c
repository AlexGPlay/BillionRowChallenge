#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "./utils/data.c"
#include "./utils/multithreading.c"

#define BUFFER_SIZE 2*1024*1024
#define INITIAL_MEASUREMENTS 1500

#define FILE_NAME "measurements.txt"
#define NTHREADS 16

#define DEBUG_LINES

char* processBuffer(char* buffer, size_t* currentProcessedBytes, size_t maxProcessSize, struct NamedNode **measurementsPtr, int *currentSize, int *insertedSize, struct NameLinkedList **sortedMeasurementsNamesPtr){
  char* text = buffer;
  while(1){
    char* nextLine = strchr(text, '\n');
    if(nextLine == NULL){
      return strdup(text);
    }

    struct StationData data = parseLine(text);

    addMeasurement(data.name, data.temperature, measurementsPtr, currentSize, insertedSize, sortedMeasurementsNamesPtr);

    *currentProcessedBytes += data.processedBytes;
    if(*currentProcessedBytes > maxProcessSize){
      return NULL;
    }

    text = data.nextLine + 1;
  }
}

void processChunk(void* params){
  // Params
  struct ThreadParams* parsedParams = (struct ThreadParams*)params;
  size_t from = parsedParams->from;
  size_t size = parsedParams->size;

  // Local variables
  size_t processedSize = 0;
  int hashSize = INITIAL_MEASUREMENTS;
  int insertedSize = 0;

  struct NamedNode *measurements = (struct NamedNode*)malloc(hashSize * sizeof(struct NamedNode));
  struct NameLinkedList *sortedMeasurementsNames = NULL;

  struct ReturnData* returnData = malloc(sizeof(struct ReturnData));

  // Open file and get ready to read
  FILE* file = fopen(FILE_NAME, "r");

  char *buffer;
  char *overflow = NULL;

  if(from > 0){
    fseek(file, from - 1, SEEK_SET);

    char* readData = (char*)malloc(BUFFER_SIZE);
    size_t readSize = fread(readData, 1, BUFFER_SIZE, file);

    buffer = (char*)malloc(readSize + 1);
    memcpy(buffer, readData, readSize);
    buffer[readSize] = '\0';

    if(buffer[0] != '\n'){
      char* nextLine = strchr(buffer, '\n');
      buffer = nextLine + 1;
    }
    else{
      buffer++;
    }
    
    overflow = processBuffer(buffer, &processedSize, size, &measurements, &hashSize, &insertedSize, &sortedMeasurementsNames);
  }

  while(1){
    char* readData = (char*)malloc(BUFFER_SIZE);
    size_t readSize = fread(readData, 1, BUFFER_SIZE, file);
    if(readSize == 0){
      break;
    }
    buffer = (char*)malloc(readSize + 1);
    memcpy(buffer, readData, readSize);
    buffer[readSize] = '\0';

    if(overflow){
      char *newData = (char*)malloc(readSize + strlen(overflow) + 1);
      memcpy(newData, overflow, strlen(overflow));
      memcpy(newData + strlen(overflow), buffer, readSize);
      free(overflow);
      buffer = newData;
      overflow = NULL;
    }

    overflow = processBuffer(buffer, &processedSize, size, &measurements, &hashSize, &insertedSize, &sortedMeasurementsNames);

    if(processedSize > size){
      break;
    }
  }

  if(overflow){
    processBuffer(overflow, &processedSize, size, &measurements, &hashSize, &insertedSize, &sortedMeasurementsNames);
  }

  returnData->measurements = measurements;
  returnData->sortedMeasurementsNames = sortedMeasurementsNames;

  parsedParams->returnData = returnData;

  fclose(file);
}

int main(){
  int size = INITIAL_MEASUREMENTS;
  int insertedSize = 0;

  struct stat statbuf;

  int fd = open(FILE_NAME, O_RDONLY);
  fstat(fd, &statbuf);
  close(fd);

  pthread_t threads[NTHREADS];
  size_t chunkSize = statbuf.st_size / NTHREADS;

  struct ThreadParams params[NTHREADS];
  for(int i=0;i<NTHREADS;i++){
    params[i].id = i;
    params[i].from = i * chunkSize;
    params[i].size = chunkSize;

    pthread_create(&threads[i], NULL, processChunk, &params[i]);
  }

  for (int i = 0; i < NTHREADS; i++){
    pthread_join(threads[i], NULL);
  }

  struct NameLinkedList *mergedMeasurementsNames = NULL;
  struct NamedNode *mergedMeasurements = (struct NamedNode*)malloc(INITIAL_MEASUREMENTS * sizeof(struct NamedNode));

  for(int i = 0; i < NTHREADS; i++){
    mergeResults(&mergedMeasurements, &mergedMeasurementsNames, &params[i].returnData->measurements, &params[i].returnData->sortedMeasurementsNames, INITIAL_MEASUREMENTS);
  }

  printMeasurements(mergedMeasurementsNames, mergedMeasurements, INITIAL_MEASUREMENTS);

  return 0;
}