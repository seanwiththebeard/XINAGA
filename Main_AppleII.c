#include "xinaga.h"
//#link "Demo.c"
//#link "Graphics.c"
//#link "Screen_Map.c"
#if defined(__APPLE2__)
//#resource "apple2-hgr2.cfg"
#define CFGFILE apple2-hgr2.cfg
#endif
#if defined(__C64__)
//#resource "C64.cfg"
//#define CFGFILE C64.cfg
#endif

void Demo(void);

void main()
{
  Demo();
}
