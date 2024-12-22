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

//#resource "apple2-xinaga.cfg"
//#resource "apple2.rom"
#define CFGFILE apple2-xinaga.cfg

#pragma code-name (push, "LOWCODE")
void main()
{  
  Demo();
}



