#include "Xinaga.h"
#include "GameData.h"
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
//#link "chr_generic.s"
//#resource "neschars.bin"
//#resource "tileset.bin"
//#resource "crt0.o"

void main(void)
{  
  #include <_heap.h>
  int *heaporg = (int*)&_heaporg;
  int *heapptr = (int*)&_heapptr;
  int *heapend = (int*)&_heapend;
  heaporg[0] = 0x7000; //heaporg
  heapptr[0] = heaporg[0]; //heapptr
  heapend[0] = 0x8000; //heapend
  memset((int*)heaporg[0], 0, heapend[0] - heaporg[0]); 
  
  MMC3_WRAM_ENABLE();
  //Program Banks
  MMC3_PRG_8000(1); //CPU $8000-$9FFF (or $C000-$DFFF): 8 KB switchable PRG ROM bank
  MMC3_PRG_A000(31); //CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
  //Backgrounds
  MMC3_CHR_0000(0); 	//PPU $0000-$07FF (or $1000-$17FF): 2 KB switchable CHR bank
  MMC3_CHR_0800(2); 	//PPU $0800-$0FFF (or $1800-$1FFF): 2 KB switchable CHR bank
  //Sprites
  MMC3_CHR_1000(0); 	//PPU $1000-$13FF (or $0000-$03FF): 1 KB switchable CHR bank
  MMC3_CHR_1400(0); 	//PPU $1400-$17FF (or $0400-$07FF): 1 KB switchable CHR bank
  MMC3_CHR_1800(0); 	//PPU $1800-$1BFF (or $0800-$0BFF): 1 KB switchable CHR bank
  MMC3_CHR_1C00(0);	//PPU $1C00-$1FFF (or $0C00-$0FFF): 1 KB switchable CHR bank  
  
  currentScreen = Map;
  //currentScreen = EditParty;
  
  Demo();
}

