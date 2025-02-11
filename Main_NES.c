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

const char ATTRIBUTE_TABLE[0x40] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 0-3
  0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, // rows 4-7
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, // rows 8-11
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // rows 12-15
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // rows 16-19
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, // rows 20-23
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, // rows 24-27
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f  // rows 28-29
};

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[16] = { 
  0x0F,			// screen color

  0x26,0x1C,0x2D,0x00,	// background palette 0
  0x30,0x20,0x00,0x00,	// background palette 1
  0x30,0x30,0x30,0x00,	// background palette 2
  0x30,0x30,0x30        // background palette 3
};

void UploadCharset()
{
  int x = 0;
  unsigned char *chrdata = (unsigned char*)0xA000;  
  MMC3_PRG_A000(31); //CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
  vram_adr(0);
  for (x = 0; x < 0x2000; ++x)
  {
    vram_put(chrdata[x]);
  }
}

#include <_heap.h>
  int *heaporg = (int*)&_heaporg;
  int *heapptr = (int*)&_heapptr;
  int *heapend = (int*)&_heapend;

#pragma code-name (push, "STARTUP")
#pragma rodata-name (push, "STARTUP")

void main(void) //Must be in $E000-$FFFF??
{
  /*
  PRG Banks
  Bit 6 of the last value written to $8000 swaps the PRG windows at $8000 and $C000. 
  The MMC3 uses one map if bit 6 was cleared to 0 (value & $40 == $00) and another if set to 1 (value & $40 == $40).
  
  PRG map mode →	$8000.D6 = 0	$8000.D6 = 1
  CPU Bank	Value of MMC3 register
  $8000-$9FFF	R6	(-2)
  $A000-$BFFF	R7	R7
  $C000-$DFFF	(-2)	R6
  $E000-$FFFF	(-1)	(-1)
  (-1) : the last bank
  (-2) : the second last bank
  
  Because the values in R6, R7, and $8000 are unspecified at power on, 
  the reset vector must point into $E000-$FFFF,
  and code must initialize these before jumping out of $E000-$FFFF.
  
  
  8001h, 8000h
  Register 8000h is the "control" register, while 8001h is the "data" register.
  First, a byte is written into 8000h to select the desired bank register(s).
  Then, the desired bank number can be written into 8001h.
  
  8000h:
  7  bit  0
  ---------
  CSxx xMMM
  
  //Select four 1k slots for background or sprites and two 2k slots for the other
  C: CHR Address Invert.  When set, does an effective XOR of 1000h with the CHR addresses.
  
  //Select the position of the two selectable page windows (1 and 2 or 2 and 3)
  //Bank 4 E000 will always be the last 8k of the ROM
  //This register determines where the second to last 8k of the ROM will be placed
  S: PRG ROM swapping control.
  0 - 8000-9FFFh and A000-BFFFh can be swapped out while C000-FFFFh is fixed.
  1 - A000-BFFFh and C000-DFFFh can be swapped out while 8000-9FFFh is fixed.
  When S = 0, C000-DFFF will contain the second to last bank of ROM.
  When S = 1, 8000-9FFF will contain the second to last bank of ROM.  
  E000-FFFF is always fixed to the last bank of ROM.
  */
  
  //Program Banks
  MMC3_PRG_8000(0); //CPU $8000-$9FFF (or $C000-$DFFF): 8 KB switchable PRG ROM bank
  MMC3_PRG_A000(0); //CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
  //Backgrounds
  MMC3_CHR_0000(0); 	//PPU $0000-$07FF (or $1000-$17FF): 2 KB switchable CHR bank
  MMC3_CHR_0800(2); 	//PPU $0800-$0FFF (or $1800-$1FFF): 2 KB switchable CHR bank
  //Sprites
  MMC3_CHR_1000(4); 	//PPU $1000-$13FF (or $0000-$03FF): 1 KB switchable CHR bank
  MMC3_CHR_1400(5); 	//PPU $1400-$17FF (or $0400-$07FF): 1 KB switchable CHR bank
  MMC3_CHR_1800(6); 	//PPU $1800-$1BFF (or $0800-$0BFF): 1 KB switchable CHR bank
  MMC3_CHR_1C00(7);	//PPU $1C00-$1FFF (or $0C00-$0FFF): 1 KB switchable CHR bank
  
  heaporg[0] = 0x7000; //heaporg
  heapptr[0] = heaporg[0]; //heapptr
  heapend[0] = 0x8000; //heapend
  memset((int*)heaporg[0], 0, heapend[0] - heaporg[0]); 
  
  MMC3_WRAM_ENABLE();
  
  
  currentScreen = Map;
  pal_clear();
  pal_bg(PALETTE);
  
  UploadCharset();
  DebugGraphics();
  //Demo();
}