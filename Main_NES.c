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
#define NES_PRG_BANKS 16	// # of 16KB PRG banks
#define NES_CHR_BANKS 0	// # of 8KB CHR banks

//#resource "nesbanked.cfg"
#define CFGFILE nesbanked.cfg
//#link "chr_generic.s"
//#resource "neschars.bin"
//#resource "tileset.bin"
////#resource "crt0.o"

//#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")

void PPU_Color(byte monochrome, byte red, byte green, byte blue)
{
  char value = 0b00011110;
  value |= (monochrome);
  value |= (red << 5);
  value |= (green << 6);
  value |= (blue << 7);
  ppu_mask(value);
}

char ATTRIBUTE_TABLE[0x40] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 0-3
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 4-7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 8-11
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 12-15
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 16-19
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 20-23
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 24-27
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // rows 28-29
  };

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[16] = { 
  0x0F,			// screen color

  0x26,0x1C,0x2D,0x00,	// background palette 0
  0x00,0x2D,0x30,0x00,	// background palette 1
  0x0C,0x1A,0x17,0x00,	// background palette 2
  0x0C,0x1A,0x30        // background palette 3
  };

void UploadCharset()
{
  int x = 0;
  unsigned char *chrdata = (unsigned char*)0xA000;
  MMC3_PRG_A000(26); //CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
  pal_col(0, 12);
  ppu_off();
  vram_adr(0);
  vram_write(&chrdata[x], 0x2000);
  //vram_adr(NTADR_A(0,0));
  //vram_fill(0, ROWS*COLS);
  

  ppu_on_all();
  pal_col(0, 0x0f);
  
  //vrambuf_put(NTADR_A(0,0)+(ROWS*COLS), &ATTRIBUTE_TABLE[0], 64);
  wait_vblank(1);  
}



#include <_heap.h>
int *heaporg = (int*)&_heaporg;
int *heapptr = (int*)&_heapptr;
int *heapend = (int*)&_heapend;

void main(void) //Must be in $E000-$FFFF??
{
  pal_col(0, 4);

  //Program Banks
  MMC3_PRG_8000(0); //CPU $8000-$9FFF (or $C000-$DFFF): 8 KB switchable PRG ROM bank
  MMC3_PRG_A000(0); //CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
  MMC3_WRAM_ENABLE();
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
  //memset((int*)heaporg[0], 0, heapend[0] - heaporg[0]); 

  InitializeGraphics();
  UploadCharset();

  //PPU_Color(0, 0, 1, 0);

  currentScreen = MapGen;
  //Demo();

  //DrawCharset();
  LoadMap();
  DrawMiniMap(false);

  //ResizeMessageWindow(consolePosX, ROWS - 9, consoleWidth, 6);

  SetAttrib(0, 0, 2);  
  SetAttrib(2, 2, 2);
  SetAttrib(4, 4, 2);
  
  //SetAttrib(12, 0, 2);
  //SetAttrib(20, 0, 2);
  //SetAttrib(24, 0, 2);
  while(1)
  {
    byte x, y, z;
    for (z = 0; z < 4; ++z)
    {
      for (y = viewportPosY; y < viewportPosY + 64; y+=2)
        for (x = viewportPosX; x < viewportPosX + 64; x+=2)
          SetAttrib(x, y, z);
      UpdateAttributes();
    }
    

    //WriteLineMessageWindow("The Quick Brown Fox Jumps Over The Lazy Dog@", 0);
    //WriteLineMessageWindow("ABCDEFGHIJKL MNOPQRSTUVWXYZ@", 0);
    //WriteLineMessageWindow("abdefghijklmnopqrstuvwxyz@", 0);
    //WriteLineMessageWindow("01234567890 !#$%^&@", 0);
    //WriteLineMessageWindow("*()-=[];':<>,./?@", 0);
  }

  //DebugGraphics();

}