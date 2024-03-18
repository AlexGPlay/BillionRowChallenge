#include <stdio.h>

#include "./hash.h"
#include "./data.h"

int hash(char* name, int size, int iterations){
  int hash = 0;
  for(int i = 0; i < strlen(name); i++){
    hash += name[i];
  }
  return (hash + iterations) % size;
}

int findPositionInHash(char* name, struct NamedNode *measurements, int size){
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

int addToHash(char* name, int value, struct NamedNode* measurements, int size){
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