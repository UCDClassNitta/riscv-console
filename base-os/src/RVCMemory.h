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



SMemoryPoolFreeChunkRef allocateFreeChunk(void);
void deallocByChunk(SMemoryPoolFreeChunkRef chunk);

#endif 