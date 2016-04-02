#include "list.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>

/* Adds element to list or create a new list if empty
Returns true: element added
false: memory allocation error
*/
bool InsertListItem(List **list, char const *word)
{
	int wordSize = strlen(word);

	if (*list == NULL)
	{
		if (!((*list) = (List *)malloc(sizeof(List))))
		{
			printErrorMessage("Memory allocation error.", ERROR, FPATH, LINE);
			return false;
		}
	}

	else
	{
		while ((*list)->next != NULL)
		{
			*list = (*list)->next;
		}

		if (!((*list)->next = (List *)malloc(sizeof(List))))
		{
			printErrorMessage("Memory allocation error.", ERROR, FPATH, LINE);
			return false;
		}
	}

	(*list)->word = (char *)malloc(wordSize * sizeof(char) + 1);
	strcpy((*list)->word, word);

	(*list)->next = NULL;
	return true;
}

/* Removes element from list
Returns true: element deleted
false: list empty or element not found
*/
bool RemoveListItem(List **list, char const *word)
{
	if ((*list)->word == NULL && (*list)->next == NULL)
	{
		printErrorMessage("List is empty.", ERROR, FPATH, LINE);
		return false;
	}

	if (strcmp((*list)->word, word) == 0)
	{
		if ((*list)->next == NULL)
		{
			free((*list));
			*list = NULL;

			return true;
		}

		else
		{
			while (strcmp(((*list)->next)->word, word) != 0 && ((*list)->next)->next != NULL)
			{
				*list = (*list)->next;
			}

			//Not existent
			if (strcmp(((*list)->next)->word, word) != 0 && ((*list)->next)->next == NULL)
			{
				printErrorMessage("Element not existent in list.", ERROR, FPATH, LINE);
				return false;
			}

			//if last
			else if ((strcmp(((*list)->next)->word, word)) == 0 && (((*list)->next)->next == NULL))
			{
				free((*list)->next);
				(*list)->next = NULL;

				return true;
			}

			//otherwise

			List *toDelete = (List*)(*list)->next;

			(*list)->next = ((*list)->next)->next;

			free(toDelete);
			toDelete->next = NULL;

			return true;
		}
	}
}

bool ClearList(List** list)
{
	List *toDelete = *list;

	while (toDelete != NULL)
	{
		*list = (*list)->next;

		free(toDelete);
		toDelete = *list;
	}

	*list = NULL;
	return true;
}
