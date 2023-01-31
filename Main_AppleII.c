#include "xinaga.h"
//#link "Graphics.c"
//#link "Input.c"

//#link "Demo.c"
//#link "Screen_Map.c"

#if defined(__APPLE2__)
//#resource "apple2-hgr2.cfg"
#define CFGFILE apple2-hgr2.cfg
#endif

void Demo(void);

void main()
{
  Demo();
}
