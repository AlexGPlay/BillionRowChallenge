#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "./utils/data.c"
#include "./utils/multithreading.c"

#define LINE_SIZE 50
#define INITIAL_MEASUREMENTS 1500
#define FILE_NAME "measurements.txt"

#define NTHREADS 8

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

  FILE* file = fopen(FILE_NAME, "r");
  if(from > 0){
    fseek(file, from - 1, SEEK_SET);
    // If the chunk starts in the middle of a line, we need to skip it.
    char c;
    fread(&c, 1, 1, file);
    if(c != '\n'){
      char line[LINE_SIZE];
      fgets(line, LINE_SIZE, file);
      processedSize = strlen(line);
    }
  }

  char line[LINE_SIZE];
  while(fgets(line, LINE_SIZE, file) && processedSize < size){
    char *strtok_data;

    char* name = strtok_r(line, ";", &strtok_data);
    char* value = strtok_r(NULL, ";", &strtok_data);

    float fvalue = atof(value);
    addMeasurement(name, fvalue, &measurements, &hashSize, &insertedSize, &sortedMeasurementsNames);

    processedSize += strlen(line);
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