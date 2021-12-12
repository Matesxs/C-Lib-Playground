//
// Created by Martin on 12.12.2021.
//

#define _GNU_SOURCE

#include <memory.h>
#include "shared_memory.h"

#include <semaphore.h>
#include "sys/mman.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

#define SHARED_MEMORY_INIT_SIZE 10

typedef struct shared_memory_item
{
  void *pointer;
  size_t size;
} SharedMemoryItem;

typedef struct shared_memory_container
{
  uint64_t shared_memory_allocated_length;
  SharedMemoryItem *shared_memory_array;
} SMContainer;

bool initialized = false;
SMContainer *container = NULL;
sem_t *accessSemaphore = NULL;

void *get_shared_memory(size_t size)
{
  void *mem = NULL;

  if ((mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
  {
    fprintf(stderr, "[ERROR] Failed to allocate shared memory\n");
    sharedMemoryCleanup();
    exit(-4);
  }

  memset(mem, 0, size);
  return mem;
}

void add_memory_to_list(void *memory, size_t size)
{
  if (!initialized || container == NULL || container->shared_memory_array == NULL)
  {
    fprintf(stderr, "[Internal ERROR] Failed to add memory to memory list because container is not initialized correctly\n");
    return;
  }

  for (uint64_t i = 0; i < container->shared_memory_allocated_length; i++)
  {
    if (container->shared_memory_array[i].pointer == NULL)
    {
      container->shared_memory_array[i].pointer = memory;
      container->shared_memory_array[i].size = size;
      return;
    }
  }

  // If we get here then we don't have enough space
  container->shared_memory_array = (SharedMemoryItem*)get_shared_memory(sizeof(SharedMemoryItem) * (container->shared_memory_allocated_length + SHARED_MEMORY_INIT_SIZE));
  container->shared_memory_allocated_length += SHARED_MEMORY_INIT_SIZE;

  // Try again
  add_memory_to_list(memory, size);
}

void sharedMemoryInit()
{
  if (initialized) {
    fprintf(stderr, "[WARNING] Shared memory container is already initialized\n");
    return;
  }

  accessSemaphore = get_shared_memory(sizeof(sem_t));
  if (sem_init(accessSemaphore, 1, 1) == -1)
  {
    fprintf(stderr, "[ERROR] Failed to init access semaphore\n");
    munmap(accessSemaphore, sizeof(sem_t));
    accessSemaphore = NULL;
    exit(-5);
  }

  container = get_shared_memory(sizeof(SMContainer));
  container->shared_memory_array = (SharedMemoryItem*)get_shared_memory(sizeof(SharedMemoryItem) * SHARED_MEMORY_INIT_SIZE);

  container->shared_memory_allocated_length = SHARED_MEMORY_INIT_SIZE;

  initialized = true;
}

void sharedMemoryCleanup()
{
  if (accessSemaphore != NULL)
  {
    sem_destroy(accessSemaphore);
    munmap(accessSemaphore, sizeof(sem_t));
    accessSemaphore = NULL;
  }

  if (container == NULL) return;

  if (container->shared_memory_array != NULL)
  {
    for (uint64_t i = 0; i < container->shared_memory_allocated_length; i++)
    {
      if (container->shared_memory_array[i].pointer != NULL)
      {
        munmap(container->shared_memory_array[i].pointer, container->shared_memory_array[i].size);
        container->shared_memory_array[i].pointer = NULL;
        container->shared_memory_array[i].size = 0;
      }
    }

    munmap(container->shared_memory_array, sizeof(SharedMemoryItem) * container->shared_memory_allocated_length);
    container->shared_memory_array = NULL;
  }

  munmap(container, sizeof(SMContainer));
  container = NULL;
  initialized = false;
}

void* shared_malloc(size_t size)
{
  if (!initialized) return NULL;

  sem_wait(accessSemaphore);
  void *mem = get_shared_memory(size);
  add_memory_to_list(mem, size);
  sem_post(accessSemaphore);

  return mem;
}

void shared_free(void *pointer)
{
  if (!initialized) return;
  if (container == NULL || container->shared_memory_array == NULL) return;

  sem_wait(accessSemaphore);
  for (uint64_t i = 0; i < container->shared_memory_allocated_length; i++)
  {
    if (container->shared_memory_array[i].pointer == pointer)
    {
      munmap(container->shared_memory_array[i].pointer, container->shared_memory_array[i].size);
      container->shared_memory_array[i].pointer = NULL;
      return;
    }
  }
  sem_post(accessSemaphore);
}
