//#resource "C64.cfg"
#define CFGFILE C64.cfg

#include "Xinaga.h"
#include "GameData.h"

//#link "DiskFiles.c"

//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"

//#link "Game_Demo.c"
//#link "Game_System.c"
//#link "Game_Screen_AddCharacter.c"
//#link "Game_Screen_MapGen.c"
//#link "Game_Screen_Map.c"
//#link "Game_Screen_Combat.c"

void main(void)
{
  //DiskSave("hello.bin", &attributeset[0], 256);
  RunGame(EditParty);
}