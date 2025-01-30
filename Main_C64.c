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
