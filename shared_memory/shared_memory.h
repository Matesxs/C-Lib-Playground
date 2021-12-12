//
// Created by Martin on 12.12.2021.
//

#ifndef LIB_PLAYGROUND_SHARED_MEMORY_H
#define LIB_PLAYGROUND_SHARED_MEMORY_H

#include "stdint.h"

void sharedMemoryInit();
void* shared_malloc(size_t size);
void shared_free(void *pointer);
void sharedMemoryCleanup();

#define sfree(pointer) shared_free((void*)(pointer))
#define smalloc(size) shared_malloc(size)

#endif //LIB_PLAYGROUND_SHARED_MEMORY_H
