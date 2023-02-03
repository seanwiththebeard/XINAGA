#include "xinaga.h"
//#link "Graphics.c"
//#link "Input.c"
//#link "FileIO.c"

//#link "Demo.c"
//#link "Screen_Map.c"

#if defined(__APPLE2__)
//#resource "apple2-xinaga.cfg"
#define CFGFILE apple2-xinaga.cfg
#endif

void Demo(void);
void main()
{
  Demo();
}
