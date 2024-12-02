#if defined(__C64__)
//#resource "C64.cfg"
//#resource "C64.ctm"
//#resource "1bitchars.ctm"
#define CFGFILE C64.cfg
#endif

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

//void Demo(void);
void main(void)
{
  //byte x, y;
  //InitializeGraphics();
  
  //for (y = 0; y < 16; ++y)
   //for (x = 0; x < 16; ++x)
      //SetChar(x + 16*y,x, y);
  //while (1);
  Demo();
}
