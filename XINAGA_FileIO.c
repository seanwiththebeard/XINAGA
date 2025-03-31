#include "Xinaga.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#if defined(__C64__)

//#include <cbm.h>
#endif

#if defined(__APPLE2__)
#include <apple2.h>
//#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "XINAGA_FILEIO")
#pragma rodata-name (push, "XINAGA_FILEIO")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

void DiskSave(char *filename, byte *source, int size)
{
  #if defined (__C64__)
  /*
  //Append the save and replace prefix
  char name[16];
  sprintf(name, "@0:%s", filename);

  cbm_k_setlfs(0, 8, 0);
  cbm_k_setnam(name);
  cbm_k_open();
  cbm_k_save(source, source + size - 1);
  cbm_k_close(0);
  //cbm_k_clall;
  
  Commodore 64 file system records 2 bytes start adress header at the beginning of the files, but PC recorded .BIN files do not have these 2 Bytes, this difference can cause problems.
  */
  
  //https://retrocomputing.stackexchange.com/questions/25240/how-do-i-save-data-from-a-program-with-cc65
  FILE *fp;
  char data_out[] = "This is my data to save.@";
  
  //Saving
  _filetype = 's';
  if ((fp = fopen (filename, "w")) == 0) {
    WriteLineMessageWindow("File could not be opened@", 0);
    exit (1);
  }
  fwrite (source, 1, size, fp);
  fclose (fp);
  #endif
  
  #if defined(__APPLE2__)
  FILE* filepointer = fopen(filename, "wb"); //Write Binary
  fwrite((int*)source, size, 1, filepointer);
  fclose(filepointer);
  #endif
  #if defined(__NES__)
  filename;
  source;
  size;
  #endif
  
  #if defined (MSX)
  filename;
  source;
  size;
  #endif
  
  #if defined (__ATARI__)
  filename;
  source;
  size;
  #endif
}

void DiskLoad(char *filename, byte *dest, int size)
{
  #if defined(__C64__)
  /*
  cbm_k_setlfs(0, 8, 0);
  cbm_k_setnam(filename);
  cbm_k_open();
  cbm_k_load(0, dest);
  cbm_k_close(0);
  cbm_k_clall;
  */
  
  //https://retrocomputing.stackexchange.com/questions/25240/how-do-i-save-data-from-a-program-with-cc65
  FILE *fp;
  int x = 0;

  //Reading
  _filetype = 's';
  if ((fp = fopen (filename, "r")) == 0) {
    WriteLineMessageWindow ("File could not be opened@",0);
    exit (1);
  }

  while (x < size) {
    dest[x] = fgetc (fp);
    ++x;
    if (feof (fp)) {
      break;
    }
  }
  fclose (fp);
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
  size;
  #endif
  
  #if defined(__NES__)
  filename;
  dest;
  size;
  #endif
  
  #if defined(MSX)
  filename;
  dest;
  size;
  #endif
  
  #if defined (__ATARI__)
  filename;
  dest;
  size;
  #endif
}


