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

typedef struct string _string_data;
typedef _string_data *string_t;

string_t stringCreate();
void stringDestroy(string_t string);
void stringGlobalDestroy();

char *stringGetCstring(string_t string);

string_t stringInsert(string_t string, const char *src);
string_t stringSet(string_t string, const char *src);

string_t stringTrimFirst(string_t string);
string_t stringTrimLast(string_t string);

string_t stringFormat(string_t string, const char *format, ...);

string_t stringClear(string_t string);
