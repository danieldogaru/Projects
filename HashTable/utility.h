#ifndef UTILITY_H_
#define UTILITY_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR -1
#define DEBUG 1
#define SUCCESS 0

#define RESIZE_DOUBLE 0
#define RESIZE_HALF	  1

#define  FPATH (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
# define LINE __LINE__
//void printErrorMessage(char*, int);

void printErrorMessage(char const *, int );
void printErrorMessage(char const*, int, char const*, int);
bool isBlank(char const *);
#endif
