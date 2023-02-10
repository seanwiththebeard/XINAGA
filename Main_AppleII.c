#if defined(__APPLE2__)
//#resource "apple2-xinaga.cfg"
#define CFGFILE apple2-xinaga.cfg
#endif

#include "xinaga.h"
//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"

//#link "Demo.c"
//#link "Screen_Map.c"

//#link "System_MessageWindow.c"
#include "System_MessageWindow.h"

void Demo(void);
void main()
{
  Demo();
}
