#pragma once

#include "stddef.h"
#include "stdbool.h"
#include "stdarg.h"

#define INIT_STRING_SIZE 10

struct string
{
  char *buffer;
  
  size_t length;
  size_t allocSize;

  struct string *_prev_string;
  struct string *_next_string;
};

typedef struct string *_string_data;
typedef char **string_t;

char **stringCreate();
void stringDestroy(char **string);
void stringGlobalDestroy();

char *stringGetCstring(char **string);

char **stringInsert(char **string, const char *src);
char **stringSet(char **string, const char *src);

char **stringTrimFirst(char **string);
char **stringTrimLast(char **string);

char **stringFormat(char **string, const char *format, ...);

char **stringClear(char **string);
