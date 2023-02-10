#if defined(__APPLE2__)
//#resource "apple2-xinaga.cfg"
#define CFGFILE apple2-xinaga.cfg
#endif

#include "xinaga.h"
//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"

//#link "Game_Demo.c"
//#link "Game_Screen_Map.c"

//#link "XINAGA_Console.c"

void Demo(void);
void main()
{
  Demo();
}
