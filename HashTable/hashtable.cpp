#include "hashtable.h"
#include "utility.h"
#include "list.h"

/**
* Functie de hash bazata pe djb2 a lui Dan Bernstein
* http://www.cse.yorku.ca/~oz/hash.html
* @return valoarea de dispersie (cheia)
*/
unsigned int hash(const char *str, unsigned int hash_length)
{
	unsigned int hash = 5381;
	int c;

	while ((c = *str++) != 0)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return (hash % hash_length);
}

bool initializeHashTable(HashTable *hashTable)
{
	hashTable->table = (List **)malloc(hashTable->size * sizeof(List));

	int iter;
	for (iter = 0; iter < hashTable->size; iter++)
	{
		hashTable->table[iter] = (List*)malloc(sizeof(List));
		hashTable->table[iter]->word = NULL;
		hashTable->table[iter]->next = NULL;
	}

	if (!hashTable->table)
	{
		printErrorMessage("Failed hastable memory allocation.", ERROR, FPATH, LINE);
		return false;
	}

	return true;
	/*
	for (int i = 0; i < hashTable->size; ++i)
	{
		hashTable->table[i] = (List*)malloc(sizeof(List));

		if (!hashTable->table[i])
		{
			printErrorMessage("List allocation error in hashtable.", ERROR, FPATH, LINE);
			return false;
		}

		hashTable->table[i]->next = NULL;
	}

	return true;*/
}

bool findInHashTable(HashTable *hashTable, char *word, bool writeToFile, char *outputFile)
{
	if (hashTable == NULL || word == NULL)
	{
		return false;
	}

	unsigned int key = hash(word, hashTable->size);

	if (isBlank(hashTable->table[key]->word))
	{
		return false;
	}

	bool retval = false;

	while (hashTable->table[key]->next != NULL)
	{
		if (strcmp(hashTable->table[key]->word, word) == 0)
		{
			retval = true;
		}

		hashTable->table[key] = hashTable->table[key]->next;
	}

	if (!writeToFile)
	{
		if (retval == true)
		{
			printf("TRUE\n");
		}
		else
		{
			printf("FALSE\n");
		}
	}

	else
	{
		FILE *out = fopen(outputFile, "a");
		if (retval == true)
		{
			fprintf(out, "TRUE\n");
		}
		else
		{
			fprintf(out, "FALSE\n");
		}

		fclose(out);
	}

	return retval;
}

bool insertElementInHTable(HashTable* hashTable, char const *word)
{
	if (hashTable == NULL || word == NULL)
	{
		printErrorMessage("Hashtable not initialized or empty word.", ERROR, FPATH, LINE);
		return false;
	}

	if (hashTable->table && (hashTable, word, false))
	{
		printErrorMessage("Cannot add duplicate word.", ERROR, FPATH, LINE);
		return false;
	}

	else
	{
		unsigned int key = hash(word, hashTable->size);


		if (!InsertListItem(&hashTable->table[key], word))
		{
			printErrorMessage("Failed to add element into hashtable.", ERROR, FPATH, LINE);
			return false;
		}

		printErrorMessage("Added element into hashtable succesfully.", DEBUG, FPATH, LINE);
		return true;

		/*
		while (hashTable->table[key]->next != NULL)
		{
		hashTable->table[key] = hashTable->table[key]->next;
		}

		hashTable->table[key]->next = (List *) malloc(sizeof(List));
		strcpy(hashTable->table[key]->next->word, word);
		hashTable->table[key]->next->next = NULL;
		return true;*/

	}
}

bool removeElementFromHTable(HashTable *hashTable, char const *word)
{
	if (hashTable == NULL || word == NULL)
	{
		printErrorMessage("Empty hashtable or word.", ERROR, FPATH, LINE);
		return false;
	}

	unsigned int key = hash(word, hashTable->size);

	if (!RemoveListItem(&hashTable->table[key], word))
	{
		printErrorMessage("Failed removing element from hashtable.", ERROR, FPATH, LINE);
		return false;
	}

	printErrorMessage("Element removed succesfully.", DEBUG, FPATH, LINE);
	return true;
}

bool clearHashTable(HashTable *hashTable)
{
	int i;
	for (i = 0; i < hashTable->size; ++i)
	{
		if (!ClearList(&hashTable->table[i]))
		{
			return false;
		}
	}

	return true;
}

void print_bucket(HashTable *hashTable, int index_bucket, char const *outputFile)
{
	if (hashTable == NULL)
	{
		printErrorMessage("HashTable is empty.", ERROR, FPATH, LINE);
		return;
	}

	if (hashTable->table[index_bucket] == NULL || (hashTable->table[index_bucket])->word == NULL)
	{
		printErrorMessage("Cannot print empty bucket", ERROR, FPATH, LINE);
		return;
	}

	FILE *output = NULL;
	List *bucket = hashTable->table[index_bucket];

	if ( isBlank(outputFile) == false )
	{
		output = fopen(outputFile, "a");
	}

	do
	{
		if (output)
		{
			fprintf(output, "%s ", bucket->word);
		}

		else
		{
			printf("%s ", bucket->word);
		}

		bucket = bucket->next;
	} while (bucket != NULL);

	if (output)
	{
		fprintf(output, "\n");
		fclose(output);
	}
	/*else
	{
		printf("\n");
	}*/
}

void print_bucket_list(HashTable *hashTable, const char *outputFile)
{
	if (hashTable == NULL)
	{
		printErrorMessage("HashTable is empty.", ERROR, FPATH, LINE);
		return;
	}

	int i;

	for (i = 0; i < hashTable->size; i++)
	{
		print_bucket(hashTable, i, outputFile);
	}
}

HashTable* resizeHashTable(HashTable *sourceTable, int mode, int *retVal)
{
	if (sourceTable == NULL)
	{
		printErrorMessage("HashTable is NULL.", ERROR, FPATH, LINE);
		*retVal = ERROR;
		return NULL;
	}

	HashTable *targetTable = (HashTable *)malloc(sizeof(HashTable));

	if (mode == RESIZE_DOUBLE)
	{
		targetTable->size = 2 * sourceTable->size;
	}

	else if (mode == RESIZE_HALF)
	{
		targetTable->size = sourceTable->size / 2;
	}
	else
	{
		printErrorMessage("Resize failed. Invalid method", ERROR, FPATH, LINE);
		*retVal = ERROR;
		return sourceTable;
	}

	initializeHashTable(targetTable);

	int iter;
	for (iter = 0; iter < sourceTable->size; iter++)
	{
		/*if (sourceTable->table[iter] == NULL)
		{
			continue;
		}*/

		if (sourceTable->table[iter]->word == NULL && sourceTable->table[iter]->next == NULL)
		{
			continue;
		}

		else
		{
			List *bucket = (List *)sourceTable->table[iter];

			if (bucket->next == NULL)
			{
				if (!insertElementInHTable(targetTable, bucket->word))
				{
					*retVal = ERROR;
					return sourceTable;
				}
			}
			else while (bucket->next != NULL)
			{
				if (!insertElementInHTable(targetTable, bucket->word))
				{
					*retVal = ERROR;
					return sourceTable;
				}

				bucket = bucket->next;
			}
		}
	}

	if (!clearHashTable(sourceTable))
	{
		*retVal = ERROR;
		return sourceTable;
	}

	return targetTable;
}

int parseArguments(HashTable *hashTable, char *line)
{

	char **commands = (char**)malloc(sizeof(char**));
	char *token = strtok(line, " ");

	int i = 0;
	do 
	{
		commands[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
		commands[i][0] = '\0';
		strcpy(commands[i], token);
		i++;
		token = strtok(NULL, " ");
	} while (token != NULL);

	i--;
	commands[i + 1] = NULL;

	char *s = strchr(commands[i], '\n');

	if(s != NULL)
	{
		*strpbrk(commands[i], "\n") = '\0';
	}
	

	if (strcmp(commands[0], "add") == 0)
	{
		if (!insertElementInHTable(hashTable, commands[1]))
		{
			printErrorMessage("Failed adding element into hashTable", ERROR, FPATH, LINE);
			return ERROR;
		}
	}

	else if (strcmp(commands[0], "print") == 0)
	{
		print_bucket_list(hashTable, commands[1]);
	}

	else if (strcmp(commands[0], "find") == 0)
	{
		if (commands[2] == NULL)
		{

			if (!findInHashTable(hashTable, commands[1], false, NULL))
			{
				printErrorMessage("Cannot find element in table.", ERROR, FPATH, LINE);
				return ERROR;
			}
		}

		else
		{
			if (!findInHashTable(hashTable, commands[1], true, commands[2]))
			{
				printErrorMessage("Cannot find element in table.", ERROR, FPATH, LINE);
				return ERROR;
			}
		}
	}

	else if (strcmp(commands[0], "remove") == 0)
	{
		if (!removeElementFromHTable(hashTable, commands[1]))
		{
			printErrorMessage("Cannot remove element from table table.", ERROR, FPATH, LINE);
			return ERROR;
		}
	}

	else if (strcmp(commands[0], "resize") == 0)
	{

		int retVal = SUCCESS;
		hashTable = resizeHashTable(hashTable, strcmp(commands[1], "double") == 0 ? RESIZE_DOUBLE : RESIZE_HALF, &retVal);

		if (retVal)
		{
			printErrorMessage("Error resizing.", ERROR, FPATH, LINE);
			return retVal;
		}
	}

	else if (strcmp(commands[0], "print_bucket") == 0)
	{
		print_bucket(hashTable, (int)atoi(commands[1]), (char*)commands[2]);
	}
	//}

	/*if (inputFile)
	{
		fclose(inputFile);
	}*/

	return SUCCESS;
}

int main(int argc, char** argv)
{
	HashTable *hashTable = (HashTable*)malloc(sizeof(HashTable));
	hashTable->size = (int)atoi(argv[1]);

	initializeHashTable(hashTable);

	int i = 2;
	FILE *inputFile;// = fopen(argv[i], "r");

	if (argv[i] == NULL)
	{
		char *fileName = (char*)malloc(20 * sizeof(char));
		scanf("%s", fileName);

		inputFile = fopen(fileName, "r");

		char *line = (char*)malloc(50 * sizeof(char));

		while (!feof(inputFile))
		{
			//double print ???
			fgets(line, 50, inputFile);

			if (parseArguments(hashTable, line))
			{
				printErrorMessage("Parsing input file failed.", ERROR, FPATH, LINE);
				return ERROR;
			}
		}

		fclose(inputFile);

		free(line);
		free(fileName);
		fileName = NULL;
		line = NULL;

	}

	else
		for (i = 2; i <= argc; i++)
		{
			if (argv[i] != NULL)
			{
				inputFile = fopen(argv[i], "r");
				char *line = (char*)malloc(50 * sizeof(char));

				while (!feof(inputFile))
				{
					fgets(line, 50, inputFile);

					if (parseArguments(hashTable, line))
					{
						printErrorMessage("Parsing input file failed.", ERROR, FPATH, LINE);
						return ERROR;
					}
				}

				fclose(inputFile);
				free(line);
				line = NULL;
			}
		}

	return SUCCESS;
}

int testFunc()
{
	HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
	hashTable->size = 4;

	if (!initializeHashTable(hashTable))
	{
		printErrorMessage("HashTable initialization failed.", ERROR, FPATH, LINE);
		return ERROR;
	}

	if (!((insertElementInHTable(hashTable, "moldovean") &&
		insertElementInHTable(hashTable, "autist") &&
		insertElementInHTable(hashTable, "autistut"))))
	{
		return ERROR;
	}
	print_bucket_list(hashTable, "bucket_list.txt");

	int retVal = SUCCESS;

	hashTable = resizeHashTable(hashTable, RESIZE_HALF, &retVal);
	print_bucket_list(hashTable, "resized.txt");
	if (retVal != SUCCESS)
	{
		return retVal;
	}

	print_bucket_list(hashTable, NULL);

	if (!removeElementFromHTable(hashTable, "autist"))
	{
		return ERROR;
	}

	getchar();
	return SUCCESS;
}

