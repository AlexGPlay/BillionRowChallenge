#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils/data.c"

#define BUFFER_SIZE 2*1024*1024
#define INITIAL_MEASUREMENTS 1500

#define DEBUG_LINES

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

  printMeasurements(sortedMeasurementsNames, measurements, insertedSize);

  return 0;
}