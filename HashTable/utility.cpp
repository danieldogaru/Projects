# include "utility.h"


void printErrorMessage(char const *errorMessage, int errorLevel)
{
	FILE *f = fopen("ERRORLOG.txt", "a");
	fprintf(f, "[%s] %s \n", (errorLevel == -1) ? "ERROR" : "DEBUG", errorMessage);

	if (f)
	{
		fclose(f);
	}
}

void printErrorMessage(char const *errorMessage, int errorLevel, char const *file, int line)
{
	FILE *f = fopen("ERRORLOG.txt", "a");
	fprintf(f, "[%s] %s:%d %s", (errorLevel == ERROR) ? "ERROR" : "DEBUG", file, line, errorMessage);
	fprintf(f, "\n");

	if (f)
	{
		fclose(f);
	}
}

bool isBlank(char const *word)
{	
	if (word == NULL)
		return true;

	if (strlen(word) == 0)
		return true;

	return false;
}
