#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "./utils/data.c"
#include "./utils/multithreading.c"

#define INITIAL_MEASUREMENTS 1500
#define NTHREADS 16
#define FILE_NAME "measurements.txt"

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

  struct StationData parsedLine;

  if(from == 0 || *(data + from - 1) == '\n'){
    parsedLine = parseLine(line);
    addMeasurement(parsedLine.name, parsedLine.temperature, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);
  }
  
  line = nextLine + 1;

  while(line < data + from + size){
    parsedLine = parseLine(line);

    addMeasurement(parsedLine.name, parsedLine.temperature, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);
    if(parsedLine.nextLine == NULL){
      break;
    }

    line = parsedLine.nextLine + 1;
  }

  returnData->measurements = measurements;
  returnData->sortedMeasurementsNames = sortedMeasurementsNames;

  parsedParams->returnData = returnData;

  pthread_exit(NULL);
}

int main(){
  int fd;
  struct stat statbuf;

  fd = open(FILE_NAME, O_RDONLY);
  fstat(fd, &statbuf);

  pthread_t threads[NTHREADS];

  size_t chunkSize = statbuf.st_size / NTHREADS;
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
    mergeResults(&mergedMeasurements, &mergedMeasurementsNames, &params[i].returnData->measurements, &params[i].returnData->sortedMeasurementsNames, INITIAL_MEASUREMENTS);
  }

  printMeasurements(mergedMeasurementsNames, mergedMeasurements, INITIAL_MEASUREMENTS);

  return 0;
}