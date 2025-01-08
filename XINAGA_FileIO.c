#include "Xinaga.h"

#if defined(__C64__)
#include <cbm.h>
#endif

#if defined(__APPLE2__)
#include <apple2.h>
//#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
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
  FILE* filepointer = fopen(filename, "wb"); //Write Binary
  fwrite((int*)source, length, 1, filepointer);
  fclose(filepointer);
  #endif
  #if defined(__NES__)
  filename;
  source;
  length;
  #endif
  
  #if defined (MSX)
  filename;
  source;
  length;
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
  filepointer = fopen(filename, "rb"); //Read Binary
  fseek(filepointer, 0, SEEK_END); // Seek our pointer to end of file
  length = ftell(filepointer); // Now the pointer is at the length of the file
  fseek(filepointer, 0, SEEK_SET); //Move the pointer back to the beginning
  fread((int*)dest, length, 1, filepointer); //Copy the entire length to our destination
  fclose(filepointer);
  #endif
  
  #if defined(__NES__)
  filename;
  dest;
  #endif
  
  #if defined(MSX)
  filename;
  dest;
  #endif
}


