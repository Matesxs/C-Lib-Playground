#pragma once

#include <stdbool.h>
#include <stddef.h>

#define STACKDEC(T, TNAME)                                                \
  typedef struct                                                          \
  {                                                                       \
    T *items;                                                             \
    int top;                                                              \
    int allocated_number;                                                 \
  } stack_##TNAME##_t;                                                    \
                                                                          \
  bool stack_##TNAME##_init(stack_##TNAME##_t *stack);                    \
  bool stack_##TNAME##_destroy(stack_##TNAME##_t *stack);                 \
  bool stack_##TNAME##_push(stack_##TNAME##_t *stack, T item);            \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack);                        \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack);                        \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack)

STACKDEC(int,int);
