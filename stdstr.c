#include "stdstr.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

string_t lastCreatedString = NULL;

string_t stringCreate()
{
  string_t tmp = (string_t)malloc(sizeof(_string_data));
  if (tmp == NULL) return NULL;

  tmp->buffer = (char*)malloc(INIT_STRING_SIZE * sizeof(char));
  if (tmp->buffer == NULL)
  {
    free(tmp);
    return NULL;
  }

  tmp->_next_string = NULL;
  tmp->_prev_string = lastCreatedString;
  if (tmp->_prev_string != NULL)
  {
    tmp->_prev_string->_next_string = tmp;
  }

  tmp->buffer[0] = 0;

  tmp->length = 0;
  tmp->allocSize = INIT_STRING_SIZE;

  lastCreatedString = tmp;
  return tmp;
}

string_t stringCreateS(const char *string)
{
  string_t tmp = stringCreate();
  tmp = stringSet(tmp, string);
  return tmp;
}

void stringDestroy(string_t string)
{
  if (string->buffer != NULL)
  {
    free(string->buffer);
    string->buffer = NULL;
  }

  if (string->_prev_string != NULL)
  {
    string->_prev_string->_next_string = string->_next_string;
  }

  if (string->_next_string != NULL)
  {
    string->_next_string->_prev_string = string->_prev_string;
  }

  string->allocSize = 0;
  string->length = 0;

  free(string);
}

void stringGlobalDestroy()
{
  if (lastCreatedString == NULL) return;

  string_t tmp = lastCreatedString->_prev_string;
  stringDestroy(lastCreatedString);
  lastCreatedString = tmp;

  stringGlobalDestroy();
}

string_t _getMoreSpace(string_t string, size_t wantedSize)
{
  if (string == NULL) return NULL;

  if (wantedSize >= string->allocSize)
  {
    while (string->allocSize <= wantedSize)
    {
      string->allocSize += INIT_STRING_SIZE;
    }

    char* tmp = (char*)realloc(string->buffer, wantedSize);
    if (tmp == NULL)
    {
      stringDestroy(string);
      return NULL;
    }

    string->buffer = tmp;
  }

  return string;
}

string_t stringInsert(string_t string, const char *src)
{
  if (string == NULL) return false;

  size_t src_size = strlen(src);
  size_t wantedSize = src_size + string->length + 1;

  string = _getMoreSpace(string, wantedSize);
  if (string == NULL) return NULL;

  strcpy(&string->buffer[string->length], src);
  string->buffer[wantedSize - 1] = 0;
  string->length = wantedSize - 1;

  return string;
}

string_t stringSet(string_t string, const char *src)
{
  if (string == NULL) return NULL;

  string = stringClear(string);
  string = stringInsert(string, src);

  return string;
}

char *stringGetCstring(string_t string)
{
  if (string == NULL) return NULL;
  return string->buffer;
}

string_t stringTrimFirst(string_t string)
{
  if (string == NULL) return NULL;

  if (string->length == 1)
  {
    string->buffer[0] = 0;
  }
  else if (string->length > 1)
  {
    char *tmp = (char*)malloc(string->length * sizeof(char));
    if (tmp == NULL)
    {
      stringDestroy(string);
      return NULL;
    }

    strcpy(tmp, &string->buffer[1]);
    strcpy(string->buffer, tmp);
    free(tmp);

    string->buffer[string->length - 1] = 0;
  }

  string->length--;

  return string;
}

string_t stringTrimLast(string_t string)
{
  if (string == NULL) return NULL;

  if (string->length > 0)
  {
    string->buffer[string->length - 1] = 0;
    string->length--;
  }

  return string;
}

string_t stringFormat(string_t string, const char *format, ...)
{
  if (string == NULL) return NULL;

  va_list arg;

  va_start (arg, format);
  size_t length = vsnprintf(string->buffer, string->allocSize, format, arg);
  va_end (arg);

  if (length >= string->allocSize)
  {
    char *tmp = (char*)malloc((length + 1) * sizeof(char));
    if (tmp == NULL)
    {
      stringDestroy(string);
      return NULL;
    }

    va_start (arg, format);
    vsnprintf(tmp, length + 1, format, arg);
    va_end (arg);

    tmp[length] = 0;

    string = stringSet(string, tmp);
    free(tmp);
  }
  else
  {
    string->length = length;
  }

  return string;
}

string_t stringClear(string_t string)
{
  if (string == NULL) return NULL;

  if (string->allocSize > 0)
    string->buffer[0] = 0;

  string->length = 0;

  return string;
}
