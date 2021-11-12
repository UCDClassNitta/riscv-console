#include <stdint.h>
#include "RVCOS.h"

void WriteString(const char *str)
{
  const char *Ptr = str;
  while (*Ptr)
  {
    Ptr++;
  }
  RVCWriteText(str, Ptr - str);
}

void WriteInt(int val)
{
  char Buffer[16];
  int Index = 14;
  int IsNeg = 0;
  Buffer[15] = '\0';
  do
  {
    Buffer[Index] = val % 10;
    if (Buffer[Index] < 0)
    {
      Buffer[Index] = -Buffer[Index];
    }
    Buffer[Index] += '0';
    Index--;
    val /= 10;
    if (val < 0)
    {
      val = -val;
      IsNeg = 1;
    }
  } while (val);
  if (IsNeg)
  {
    Buffer[Index] = '-';
    Index--;
  }
  Buffer[Index] = ' ';
  WriteString(Buffer + Index);
}

int main()
{
  int Index, Inner, Groups;
  int *Pointers[4];
  int *MemoryBase;
  TMemoryPoolID MemoryPoolID;
  TMemorySize AvailableSpace;

  WriteString("Allocating pool: \n");
  TStatus s = RVCMemoryAllocate(256, (void **)&MemoryBase);
  WriteInt(s);
  // if(RVCOS_STATUS_SUCCESS != s){
  //     WriteString("Failed to allocate memory pool\n");
  //     return -1;
  // }
  WriteString("Done, Creating pool: \n");
  s = RVCMemoryPoolCreate(MemoryBase, 256, &MemoryPoolID);
  WriteInt(s);
  WriteString("\n");
  // if(RVCOS_STATUS_SUCCESS != s){
  //     RVCMemoryPoolDeallocate(RVCOS_MEMORY_POOL_ID_SYSTEM, MemoryBase);
  //     WriteString("Failed to create memory pool\n");
  //     return -1;
  // }
  WriteString("Done, Querying space: \n");
  s = RVCMemoryPoolQuery(MemoryPoolID, &AvailableSpace);
  WriteInt(s);
  WriteString("\n");
  WriteString("\n");
  WriteString("query result: ");
  WriteInt(AvailableSpace);
  WriteString("\n");
  // if(RVCOS_STATUS_SUCCESS != s){
  //     WriteString("Failed to query memory pool\n");
  //     RVCMemoryPoolDelete(MemoryPoolID);
  //     RVCMemoryDeallocate(MemoryBase);
  //     return -1;
  // }
  Groups = 256 == AvailableSpace ? 4 : 2;
  WriteString("Done, Allocating spaces: \n");
  for (Index = 0; Index < 4; Index++)
  {
    RVCMemoryPoolAllocate(MemoryPoolID, 64, (void **)&Pointers[Index]);
    // if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolAllocate(MemoryPoolID, 64, (void **)&Pointers[Index])){
    //     char TempStr[] = "Failed to allocate space 0\n";
    //     TempStr[25] = '0' + Index;
    //     WriteString(TempStr);
    //     return -1;
    // }
  }
  WriteString("Done, Assigning values: \n");
  for (Index = 0; Index < 4; Index++)
  {
    for (Inner = 0; Inner < 64 / sizeof(int); Inner++)
    {
      Pointers[Index][Inner] = Index * (64 / sizeof(int)) + Inner;
    }
  }
  WriteString("Try a Value: \n");
  WriteInt(Pointers[0][1]);

  // WriteString("Done, Deallocating spaces: \n");
  // for (Index = 0; Index < 4; Index++)
  // {
  //   s = RVCMemoryPoolDeallocate(MemoryPoolID, Pointers[Index]);
  //   WriteInt(s);
  //   WriteString("\n");
  //   // if (RVCOS_STATUS_SUCCESS != RVCMemoryPoolDeallocate(MemoryPoolID, Pointers[Index]))
  //   // {
  //   //   char TempStr[] = "Failed to deallocate space 0\n";
  //   //   TempStr[27] = '0' + Index;
  //   //   WriteString(TempStr);
  //   //   return -1;
  //   // }
  // }
  // WriteString("Done\nAllocating full space: ");
  // if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolAllocate(MemoryPoolID, AvailableSpace, (void **)&Pointers[0])){
  //     WriteString("Failed to allocate full space\n");
  //     return -1;
  // }
  // WriteString("Done\nPrinting values:");
  // for(Index = 0; Index < (AvailableSpace / sizeof(int)); Index++){
  //     WriteInt(Pointers[0][Index]);
  // }
  // WriteString("\nDeallocating space: ");
  // if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolDeallocate(MemoryPoolID, Pointers[0])){
  //     WriteString("Failed to deallocate full space\n");
  //     return -1;
  // }
  // WriteString("Done\nDeleting memory pool: ");
  // if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolDelete(MemoryPoolID)){
  //     WriteString("Failed to delete memory pool\n");
  //     return -1;
  // }
  // WriteString("Done\nDeallocating memory pool space: ");
  // if(RVCOS_STATUS_SUCCESS != RVCMemoryDeallocate(MemoryBase)){
  //     WriteString("Failed to deallocate memory pool space\n");
  //     return -1;
  // }
  WriteString("All Done\n");
  return 0;
}
