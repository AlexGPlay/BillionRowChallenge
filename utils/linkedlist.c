#include <stdio.h>

#include "./linkedlist.h"
#include "./data.h"

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