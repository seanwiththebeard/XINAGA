#if defined(__C64__)
#include <cbm.h>
#include <stdio.h>
#endif

#if defined(__APPLE2__)
#include <apple2.h>
#include <stdio.h>
#endif

void DiskSave(char filename[], int source, int length)
{
  #if defined (__C64__)
  //Append the save and replace prefix
  char name[16];
  sprintf(name, "@0:%s", filename);

  cbm_k_setlfs(0, 8, 0);
  cbm_k_setnam(name);
  cbm_k_open();
  cbm_k_save(source, source + length - 1);
  cbm_k_close(0);
  //cbm_k_clall;
  #endif
  
  #if defined(__APPLE2__)
  FILE* filepointer = fopen(filename, "wb");
  fwrite((int*)source, length, 1, filepointer);
  fclose(filepointer);
  #endif
}

void DiskLoad(char filename[], int dest)
{
  #if defined(__C64__)
  cbm_k_setlfs(0, 8, 0);
  cbm_k_setnam(filename);
  cbm_k_open();
  cbm_k_load(0, dest);
  cbm_k_close(0);
  //cbm_k_clall;
  #endif

  #if defined(__APPLE2__)
  FILE* filepointer;
  int length;
  filepointer = fopen(filename, "rb");
  fseek(filepointer, 0, SEEK_END); // seek to end of file
  length = ftell(filepointer); // get current file pointer
  fseek(filepointer, 0, SEEK_SET);
  fread((int*)dest, length, 1, filepointer);
  fclose(filepointer);
  #endif
}

