#include "string.h"
#include "stdstr.h"
#include "stdio.h"

int main()
{
  char **string = stringCreate();
  char **string1 = stringCreate();
  char **string2 = stringCreate();
  char **string3 = stringCreate();

  printf("%s\n", *string);
  printf("%s\n", stringGetCstring(string1));
  printf("%s\n", stringGetCstring(string2));
  printf("%s\n", stringGetCstring(string3));

  stringDestroy(string2);

  string = stringInsert(string, "l");
  printf("%s\n", *string);

  string = stringTrimFirst(string);
  printf("%s\n", *string);

  string = stringInsert(string, "l");
  printf("%s\n", *string);

  string = stringTrimLast(string);
  printf("%s\n", *string);

  string = stringFormat(string, "%s, %ld\n", "Tohle je ale taková píčovina\n", 555555);
  printf("%s\n", *string);

  string = stringClear(string);
  printf("%s\n", *string);

  string = stringInsert(string, "abc");
  printf("%s\n", *string);

  string = stringInsert(string, "xyz");
  printf("%s\n", *string);

  string = stringInsert(string, "123456789");
  printf("%s\n", *string);

  string = stringTrimFirst(string);
  printf("%s\n", *string);

  string = stringTrimLast(string);
  printf("%s\n", *string);

  string = stringFormat(string, "%d, %d, %s\n", 5, 6, "test");
  printf("%s\n", *string);

  stringGlobalDestroy();

  return 0;
}