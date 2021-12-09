#include "stdstr.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

_string_data lastCreatedString = NULL;

string_t stringCreate()
{
  _string_data tmp = (_string_data)malloc(sizeof(struct string));
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
  return (string_t)tmp;
}

string_t stringCreateS(const char *string)
{
  string_t tmp = stringCreate();
  tmp = stringSet(tmp, string);
  return tmp;
}

void stringDestroy(string_t string)
{
  _string_data tmp = (_string_data)string;

  if (tmp->buffer != NULL)
  {
    free(tmp->buffer);
    tmp->buffer = NULL;
  }

  if (tmp->_prev_string != NULL)
  {
    tmp->_prev_string->_next_string = tmp->_next_string;
  }

  if (tmp->_next_string != NULL)
  {
    tmp->_next_string->_prev_string = tmp->_prev_string;
  }

  tmp->allocSize = 0;
  tmp->length = 0;

  free(tmp);
}

void stringGlobalDestroy()
{
  if (lastCreatedString == NULL) return;

  _string_data tmp = lastCreatedString->_prev_string;
  stringDestroy((string_t)lastCreatedString);
  lastCreatedString = tmp;

  stringGlobalDestroy();
}

_string_data _getMoreSpace(_string_data string, size_t wantedSize)
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
      stringDestroy((string_t)string);
      return NULL;
    }

    string->buffer = tmp;
  }

  return string;
}

string_t stringInsert(string_t string, const char *src)
{
  _string_data tmp = (_string_data)string;

  if (tmp == NULL) return false;

  size_t src_size = strlen(src);
  size_t wantedSize = src_size + tmp->length + 1;

  tmp = _getMoreSpace(tmp, wantedSize);
  if (string == NULL) return NULL;

  strcpy(&tmp->buffer[tmp->length], src);
  tmp->buffer[wantedSize - 1] = 0;
  tmp->length = wantedSize - 1;

  return (string_t)string;
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
  return ((_string_data)string)->buffer;
}

string_t stringTrimFirst(string_t string)
{
  _string_data tmpS = (_string_data)string;

  if (tmpS == NULL) return NULL;

  if (tmpS->length == 1)
  {
    tmpS->buffer[0] = 0;
  }
  else if (tmpS->length > 1)
  {
    char *tmp = (char*)malloc(tmpS->length * sizeof(char));
    if (tmp == NULL)
    {
      stringDestroy(string);
      return NULL;
    }

    strcpy(tmp, &tmpS->buffer[1]);
    strcpy(tmpS->buffer, tmp);
    free(tmp);

    tmpS->buffer[tmpS->length - 1] = 0;
  }

  tmpS->length--;

  return (string_t)tmpS;
}

string_t stringTrimLast(string_t string)
{
  _string_data tmp = (_string_data)string;

  if (string == NULL) return NULL;

  if (tmp->length > 0)
  {
    tmp->buffer[tmp->length - 1] = 0;
    tmp->length--;
  }

  return (string_t)tmp;
}

string_t stringFormat(string_t string, const char *format, ...)
{
  _string_data tmpS = (_string_data)string;

  if (string == NULL) return NULL;

  va_list arg;

  va_start (arg, format);
  size_t length = vsnprintf(tmpS->buffer, tmpS->allocSize, format, arg);
  va_end (arg);

  if (length >= tmpS->allocSize)
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

    tmpS = (_string_data)stringSet(string, tmp);
    free(tmp);
  }
  else
  {
    tmpS->length = length;
  }

  return (string_t)tmpS;
}

string_t stringClear(string_t string)
{
  _string_data tmpS = (_string_data)string;

  if (tmpS == NULL) return NULL;

  if (tmpS->allocSize > 0)
    tmpS->buffer[0] = 0;

  tmpS->length = 0;

  return (string_t)tmpS;
}
