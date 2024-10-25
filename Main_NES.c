//#resource "crt0.o"

#include "Xinaga.h"
#include "GameData.h"
#include "neslib.h"

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
//#resource "tileset.bin"


#include <_heap.h>
int *heaporg = (int*)&_heaporg;
int *heapptr = (int*)&_heapptr;
int *heapend = (int*)&_heapend;

void setHeap()
{
  heaporg[0] = 0x7000; //heaporg
  heapptr[0] = heaporg[0]; //heapptr
  heapend[0] = 0x8000; //heapend
  memset((int*)heaporg[0], 0, heapend[0] - heaporg[0]); 
}
#define length 128

int heap_avail(void)
{
  char *t;
  char stringA[24] = "";
  int x = 1;

  while(1)
  {
    if (!(t=malloc(x))) break;
    free(t);
    ++x;
  }

  ppu_off();
  sprintf(stringA, "heap starts: $%4x", heaporg[0]);
  vram_adr(NTADR_A(2,3));
  vram_write(stringA, strlen(stringA));
  sprintf(stringA, "heap ends:   $%4x", heapend[0]);
  vram_adr(NTADR_A(2,4));
  vram_write(stringA, strlen(stringA));
  sprintf(stringA, "heap avail:   %4u bytes",x - 1);
  vram_adr(NTADR_A(2,2));
  vram_write(stringA, strlen(stringA));
  sprintf(stringA, "Last %d bytes stored @ $%4x",length, heapptr[0]);
  vram_adr(NTADR_A(2,7));
  vram_write(stringA, strlen(stringA));
  ppu_on_all();
  return x - 1;
}




byte x, y;
//void Demo(void);
void main(void)
{
  //MMC3_WRAM_ENABLE();
  
  //Program Banks
  MMC3_PRG_8000(0); //CPU $8000-$9FFF (or $C000-$DFFF): 8 KB switchable PRG ROM bank
  MMC3_PRG_A000(31); //CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
  
  //Backgrounds
  MMC3_CHR_0000(0); 	//PPU $0000-$07FF (or $1000-$17FF): 2 KB switchable CHR bank
  MMC3_CHR_0800(2); 	//PPU $0800-$0FFF (or $1800-$1FFF): 2 KB switchable CHR bank
  
  //Sprites
  MMC3_CHR_1000(0); 	//PPU $1000-$13FF (or $0000-$03FF): 1 KB switchable CHR bank
  MMC3_CHR_1400(0); 	//PPU $1400-$17FF (or $0400-$07FF): 1 KB switchable CHR bank
  MMC3_CHR_1800(0); 	//PPU $1800-$1BFF (or $0800-$0BFF): 1 KB switchable CHR bank
  MMC3_CHR_1C00(0);	//PPU $1C00-$1FFF (or $0C00-$0FFF): 1 KB switchable CHR bank
  setHeap();
  
  
  InitializeGraphics();
  ClearScreen();
  
  Demo();
  
  
  /*//heap_avail();
  
  //while (1)
  {

    byte i = 0;
    char *dataA = malloc(length);
    for (i = 0; i < length; ++i)
      dataA[i] = i + heapptr[0];


    ppu_off();
    vram_adr(NTADR_A(2,8));
    vram_write(dataA, length - 1);
    ppu_on_all();
    heap_avail();


    if (heapend[0] - (heapptr[0] + length) < 0)
      main();
    
  }*/
  
  //byte x, y;
  //InitializeGraphics();
  ClearScreen();
  for (y = 0; y < 16; ++y)
   for (x = 0; x < 16; ++x)
      SetChar(x + 16*y, x + 2, y + 2);
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
  
  while(1)
  {
  };
}

