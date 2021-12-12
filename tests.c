//
// Created by Martin on 12.12.2021.
//

#include "tests.h"

#include <unistd.h>
#include "stdio.h"
#include "stdstr/stdstr.h"
#include "shared_memory/shared_memory.h"
#include "stack/stack.h"

void stdlib_test()
{
  string_t string = stringCreate();
  string_t string1 = stringCreateS("Some static string");
  string_t string2 = stringCreateS("String that will get destroyed");
  string_t string3 = stringCreateF("%s and some other shit %d, %f\n", "Some dinamicaly formated string", 123456, 14.666);

  printf("%s\n", *string);
  printf("%s\n", stringGetCstring(string1));
  printf("%s\n", stringGetCstring(string2));
  printf("%s\n", *string3);

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
}

void shared_mem_test()
{
  sharedMemoryInit();

  int *test1 = smalloc(sizeof(int) * 500);
  test1[1] = 5555;

  int *test2 = smalloc(sizeof(int) * 500);
  test2[1] = 5555;

  int *test3 = smalloc(sizeof(int) * 500);
  test3[1] = 5555;

  int *test4 = smalloc(sizeof(int) * 500);
  test4[1] = 5555;

  int *test5 = smalloc(sizeof(int) * 500);
  test5[1] = 5555;

  int *test6 = smalloc(sizeof(int) * 500);
  test6[1] = 5555;

  int *test7 = smalloc(sizeof(int) * 500);
  test7[1] = 5555;

  int *test8 = smalloc(sizeof(int) * 500);
  test8[1] = 5555;

  int *test9 = smalloc(sizeof(int) * 500);
  test9[1] = 5555;

  int *test10 = smalloc(sizeof(int) * 500);
  test10[1] = 5555;

  if (fork() == 0)
  {
    // Here is forked process
    fprintf(stderr, "Child: %d\n", test1[1]);
    test1[1] = 3;
    return;
  }

  int *test11 = smalloc(sizeof(int) * 500);
  test11[1] = 5555;

  int *test12 = smalloc(sizeof(int) * 500);
  test12[1] = 5555;

  int *test13 = smalloc(sizeof(int) * 500);
  test13[1] = 5555;

  int *test14 = smalloc(sizeof(int) * 500);
  test14[1] = 5555;

  sleep(1);

  fprintf(stderr, "Parent: %d\n", test1[1]);

  sfree(test1);
  sfree(test2);

  sharedMemoryCleanup();
}

void stack_test()
{
  stack_int_t stack;
  stack_int_init(&stack);

  stack_int_push(&stack, 1);
  stack_int_push(&stack, 2);
  stack_int_push(&stack, 3);

  fprintf(stderr, "%d\n", stack_int_top(&stack));
  fprintf(stderr, "%d\n", stack_int_pop(&stack));
  fprintf(stderr, "%d\n", stack_int_pop(&stack));
  fprintf(stderr, "%d\n", stack_int_pop(&stack));
  fprintf(stderr, "%d\n", stack_int_pop(&stack));
  fprintf(stderr, "%d\n", stack_int_empty(&stack));

  stack_int_destroy(&stack);
}
