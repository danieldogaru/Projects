#pragma once

#include "list.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct HashTable_
{
	unsigned int size;
	List **table;
} HashTable;

unsigned int hash(const char *str, unsigned int hash_length);

bool initializeHashTable(HashTable*);
bool findInHashTable(HashTable *, char *, bool, char*);
bool insertElementInHTable(HashTable *, const char *);
bool removeElementFromHTable(HashTable *, const char *);
bool clearHashTable(HashTable *);

void print_bucket(HashTable *, int, const char *);
void print_bucket_list(HashTable *, const char *);
int  parseArguments(HashTable*, char*);







