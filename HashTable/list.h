#ifndef LIST_H_
#define LIST_H_
#include "utility.h"
#include <stdio.h>
#include <string.h>

typedef struct List
{
	char *word;
	List *next;
} List;

bool InsertListItem(List **, char const *);
bool RemoveListItem(List **, char const *);
bool ClearList(List **);
#endif
