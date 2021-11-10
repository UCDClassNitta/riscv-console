#ifndef RVCOSMEMORY_H
#define RVCOSMEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MIN_ALLOC_SIZE 0x40

typedef struct FREE_NODE FreeNode, *FreeNodeRef; // fixed size

struct FREE_NODE
{
  struct FREE_NODE *next;
};

typedef struct
{
  int count;
  int struct_size;
  FreeNodeRef first_free;
} Allocator, *AllocatorRef;
void *private_memAlloc(int size);

void allocatorInit(AllocatorRef alloc, int size);
void *allocate(AllocatorRef alloc);
void deallocate(AllocatorRef alloc, void *obj);

typedef struct
{
  int DX;
  int DY;
  int DZ;
} Vec3, *Vec3Ref;

typedef struct
{
  int DSize;
  void *DBase;
} SMemoryPoolFreeChunk, *SMemoryPoolFreeChunkRef;

Allocator sys_mem_allocator;
int alloc_is_suspended = 0;
SMemoryPoolFreeChunk initial_free_chunks[5];

SMemoryPoolFreeChunkRef allocateFreeChunk(void);
void deallocByChunk(SMemoryPoolFreeChunkRef chunk);

// int main(int argc, char *argv[])
// {
//   SMemoryPoolFreeChunkRef Array[MIN_ALLOC_SIZE * 2];

//   allocatorInit(&sys_mem_allocator, sizeof(SMemoryPoolFreeChunk));
//   for (int Index = 0; Index < 5; Index++)
//   {
//     deallocByChunk(&initial_free_chunks[Index]);
//   }
//   for (int Index = 0; Index < MIN_ALLOC_SIZE * 2; Index++)
//   {
//     Array[Index] = allocateFreeChunk();
//     printf("%d => %p\n", Index, Array[Index]);
//   }
//   return 0;
// }

void *private_memAlloc(int size)
{
  printf("@line %d \n", __LINE__);
  allocateFreeChunk();
  return malloc(size);
}

void allocatorInit(AllocatorRef alloc, int size)
{
  alloc->count = 0;
  alloc->struct_size = size;
  alloc->first_free = NULL;
}

void *allocate(AllocatorRef alloc)
{
  if (!alloc->count)
  {
    alloc->first_free = (FreeNodeRef)private_memAlloc(alloc->struct_size * MIN_ALLOC_SIZE);
    FreeNodeRef Current = alloc->first_free;
    for (int Index = 0; Index < MIN_ALLOC_SIZE; Index++)
    {
      if (Index + 1 < MIN_ALLOC_SIZE)
      {
        Current->next = (FreeNodeRef)((uint8_t *)Current + alloc->struct_size);
        Current = Current->next;
      }
      else
      {
        Current->next = NULL;
      }
    }
    alloc->count = MIN_ALLOC_SIZE;
  }
  FreeNodeRef NewStruct = alloc->first_free;
  alloc->first_free = alloc->first_free->next;
  alloc->count--;
  return NewStruct;
}

void deallocate(AllocatorRef alloc, void *obj)
{
  FreeNodeRef OldStruct = (FreeNodeRef)obj;
  alloc->count++;
  OldStruct->next = alloc->first_free;
  alloc->first_free = OldStruct;
}

SMemoryPoolFreeChunkRef allocateFreeChunk(void)
{
  if (3 > sys_mem_allocator.count && !alloc_is_suspended)
  {
    alloc_is_suspended = 1;
    printf("@line %d \n", __LINE__);
    uint8_t *Ptr = (uint8_t*)private_memAlloc(sys_mem_allocator.struct_size * MIN_ALLOC_SIZE);
    for (int Index = 0; Index < MIN_ALLOC_SIZE; Index++)
    {
      deallocate(&sys_mem_allocator, Ptr + Index * sys_mem_allocator.struct_size);
    }
    alloc_is_suspended = 0;
  }
  return (SMemoryPoolFreeChunkRef)allocate(&sys_mem_allocator);
}

void deallocByChunk(SMemoryPoolFreeChunkRef chunk)
{
  deallocate(&sys_mem_allocator, (void *)chunk);
}

#endif 