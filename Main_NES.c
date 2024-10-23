#include "Xinaga.h"
#include "GameData.h"
#include "neslib.h"
#include <_heap.h>

// VRAM buffer module
#include "vrambuf.h"
//#link "vrambuf.c"


// bank-switching configuration
#define NES_MAPPER 4		// Mapper 4 (MMC3)
#define NES_PRG_BANKS 32	// # of 16KB PRG banks
#define NES_CHR_BANKS 32	// # of 8KB CHR banks

//#resource "nesbanked.cfg"
#define CFGFILE nesbanked.cfg

//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"

//#link "Game_Demo.c"
//#link "Game_System.c"

//#link "Game_Screen_Title.c"
//#link "Game_Screen_AddCharacter.c"
//#link "Game_Screen_Map.c"
//#link "Game_Screen_Combat.c"
//#link "Game_Screen_MapGen.c"
//#link "Game_Screen_Scenario.c"

//#link "chr_generic.s"
//#resource "neschars.bin"



void heap_avail(void)
{
  int x;
  char *t;
  char stringA[24] = "";
  
  int *heaporg = (int*)&_heaporg;
  int *heapptr = (int*)&_heapptr;
  int *heapend = (int*)&_heapend;
  
  heaporg[0] = 0x7000; //heaporg
  heapptr[0] = heaporg[0]; //heapptr
  heapend[0] = 0x8000; //heapend
  
  sprintf(stringA, "heap starts: $%x", heaporg[0]);
    vram_adr(NTADR_A(2,3));
    vram_write(stringA, strlen(stringA));
  
  sprintf(stringA, "heap ends:   $%x", heapend[0]);
    vram_adr(NTADR_A(2,4));
    vram_write(stringA, strlen(stringA));
  
  x=1;
  while(1)
  {
    t=malloc(x);
    if ( !t ) break;
    free(t);
    ++x;
  }
  //if ( x > 10 ) 
    //x-=10;
  //else
    //x=0;
  
  sprintf(stringA, "heap avail:   %u bytes",x - 1);
    vram_adr(NTADR_A(2,2));
    vram_write(stringA, strlen(stringA));
  
  
}



byte x, y;
//void Demo(void);
void main(void)
{
  MMC3_WRAM_ENABLE();
  
  MMC3_PRG_8000(0);
  MMC3_PRG_A000(31);

  //MMC3_CHR_0000(0);


  //byte x, y;
  InitializeGraphics();
  ClearScreen();
  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
      SetChar(x + 16*y,x, y);
  //heap_avail();
  //InitializeGraphics();
  //ClearScreen();    
  //ResizeMessageWindow(3, 3, 12, 7);
  
  //ResetMenu("Edit Party@",1, 1, 16, 9, 9);
  //SetMenuItem(0, "Create@");
  //SetMenuItem(1, "Delete@");
  //SetMenuItem(2, "Add@");
  //SetMenuItem(3, "Remove@");
  //SetMenuItem(4, "Begin Adventure@");
  //SetMenuItem(5, "Credits@");
  //SetMenuItem(6, "Combat@");
  //SetMenuItem(7, "Map Gen@");
  //SetMenuItem(8, "Scenario Gen@");
  
  
      
  //Demo();
  while(1){};
}

