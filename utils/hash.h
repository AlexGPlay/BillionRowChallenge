#pragma once

#include "./data.h"

int hash(char* name, int size, int iterations);
int findPositionInHash(char* name, struct NamedNode *measurements, int size);
int getAvailablePosition(char* name, struct NamedNode *measurements, int size);
int addToHash(char* name, int value, struct NamedNode* measurements, int size);