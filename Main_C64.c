#if defined(__C64__)
//#resource "C64.cfg"
#define CFGFILE C64.cfg
#endif

#include "xinaga.h"
#include "game.h"
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
//#link "Game_Screen_Credits.c"

void Demo(void);
void main()
{
  Demo();
}
