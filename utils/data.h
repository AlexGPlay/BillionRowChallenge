#pragma once

struct StationData{
  char* name;
  float temperature;
  char* nextLine;
  size_t processedBytes;
};

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

void addMeasurement(char* name, float value, struct NamedNode **measurementsPtr, int* currentSize, int* insertedSize, struct NameLinkedList **sortedMeasurementsNamesPtr);
struct StationData parseLine(char* line);
void printMeasurements(struct NameLinkedList *names, struct NamedNode *measurements, int measurementsSize);