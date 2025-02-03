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

#if defined(__C64__)
//#resource "C64.cfg"
//#resource "c64charset.bin"
//#resource "c64attributeset.bin"
//#link "chr_c64.s"
#include "C64Charmap.h"
#define CFGFILE C64.cfg
#endif

void main(void)
{
  DebugGraphics();
  Demo();
}

/*{pal:"c64",layout:"c64"}*/
const char PALETTEC64[16] = { 
  0x00,			// screen color

  0x00,0x02,0x03,0x04,	// background palette 0
  0x05,0x06,0x07,0x08,	// background palette 1
  0x09,0x0A,0x0B,0x0C,	// background palette 2
  0x0D,0x0E,0x0F        // background palette 3
};