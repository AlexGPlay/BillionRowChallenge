#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "./utils/data.c"

#define INITIAL_MEASUREMENTS 1500

int main(){
  int fd;
  char *data;
  struct stat statbuf;

  #ifdef DEBUG_LINES
  int lines = 0;
  #endif

  int currentSize = INITIAL_MEASUREMENTS;
  int insertedSize = 0;

  struct NamedNode *measurements;
  measurements = (struct NamedNode*)malloc(currentSize * sizeof(struct NamedNode));

  struct NameLinkedList *sortedMeasurementsNames = NULL;

  fd = open("measurements.txt", O_RDONLY);
  fstat(fd, &statbuf);
  data = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  char* line = data;
  while(line < data + statbuf.st_size){
    char* nextLine = strchr(line, '\n');
    int bytes = nextLine - line;

    if(nextLine == NULL){
      char *separator = strchr(line, ';');
      if(separator == NULL){
        break;
      }

      int bytesUntilSeparator = separator - line;
      char *name = (char*)malloc(bytesUntilSeparator + 1);
      strncpy(name, line, bytesUntilSeparator);

      char *value = separator + 1;
      float fvalue = atof(value);

      addMeasurement(name, fvalue, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);
      break;
    }

    char* lineCopy = (char*)malloc(bytes + 1);
    strncpy(lineCopy, line, bytes);
    lineCopy[bytes] = '\0';
    
    char *name = strtok(lineCopy, ";");

    char *value = strtok(NULL, ";");
    float fvalue = atof(value);

    addMeasurement(name, fvalue, &measurements, &currentSize, &insertedSize, &sortedMeasurementsNames);
    #ifdef DEBUG_LINES
    lines++;
    if(lines % 1000000 == 0){
      printf("Lines: %d\n", lines);
    }
    #endif

    line = nextLine + 1;
  }


  munmap(data, statbuf.st_size);
  close(fd);

  printMeasurements(sortedMeasurementsNames, measurements, insertedSize);

  return 0;
}