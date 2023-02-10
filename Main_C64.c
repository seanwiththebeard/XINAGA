#if defined(__C64__)
//#resource "C64.cfg"
#define CFGFILE C64.cfg
#endif

#include "xinaga.h"
//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"


//#link "Demo.c"
//#link "Screen_Map.c"

//#link "System_MessageWindow.c"
#include "System_MessageWindow.h"

void Demo(void);
void main()
{
  Demo();
}
