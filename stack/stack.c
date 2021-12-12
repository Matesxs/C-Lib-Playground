#include "stack.h"

#define INITIAL_SIZE 20

#define STACKDEF(T, TNAME)                                                                       \
  bool stack_##TNAME##_init(stack_##TNAME##_t *stack)                                            \
  {                                                                                              \
    stack->top = -1;                                                                             \
    stack->items = (T *)malloc(INITIAL_SIZE * sizeof(T));                                        \
    if (stack->items == NULL)                                                                    \
      return false;                                                                              \
    stack->allocated_number = INITIAL_SIZE;                                                      \
    return true;                                                                                 \
  }                                                                                              \
                                                                                                 \
  bool stack_##TNAME##_destroy(stack_##TNAME##_t *stack)                                         \
  {                                                                                              \
    if (!stack_##TNAME##_empty(stack))                                                           \
      return false;                                                                              \
    if (stack->items == NULL)                                                                    \
      return false;                                                                              \
    free(stack->items);                                                                          \
    stack->items = NULL;                                                                         \
    stack->allocated_number = 0;                                                                 \
    stack->top = -1;                                                                             \
    return true;                                                                                 \
  }                                                                                              \
                                                                                                 \
  bool stack_##TNAME##_push(stack_##TNAME##_t *stack, T item)                                    \
  {                                                                                              \
    if ((stack->top + 1) >= stack->allocated_number)                                             \
    {                                                                                            \
      T *tmp = (T *)realloc(stack->items, (stack->allocated_number + INITIAL_SIZE) * sizeof(T)); \
      if (tmp == NULL)                                                                           \
        return false;                                                                            \
      stack->allocated_number = stack->allocated_number + INITIAL_SIZE;                          \
      stack->items = tmp;                                                                        \
    }                                                                                            \
    stack->items[++stack->top] = item;                                                           \
    return true;                                                                                 \
  }                                                                                              \
                                                                                                 \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack)                                                \
  {                                                                                              \
    if (stack->top == -1)                                                                        \
    {                                                                                            \
      return 0;                                                                                  \
    }                                                                                            \
    return stack->items[stack->top];                                                             \
  }                                                                                              \
                                                                                                 \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack)                                                \
  {                                                                                              \
    if (stack->top == -1)                                                                        \
    {                                                                                            \
      return 0;                                                                                  \
    }                                                                                            \
    return stack->items[stack->top--];                                                           \
  }                                                                                              \
                                                                                                 \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack)                                           \
  {                                                                                              \
    return stack->top == -1;                                                                     \
  }
