#include "xinaga.h"
#include "Common.h"

//#link "Graphics.c"
//#link "Common.c"
#if defined(__APPLE2__)
#define CFGFILE apple2-hgr2.cfg
//#resource "apple2-hgr2.cfg"
#endif

void main()
{
  byte x, y, z;
  byte i = 0;
  InitializeGraphics();
  ClearScreen();

  DrawBorder("Character Set@", 0, 0, 18, 18, false);
  DrawBorder("Tiles@", 18, 0, 18, 18, false);
  #if defined(__APPLE2__)
  DrawBorder("Hello Apple!@", 0, ROWS - 3, COLS, 2, false);
  #endif
  #if defined(__C64__)
  DrawBorder("Hello C64!@", 0, ROWS - 3, COLS, 3, false);
  #endif
  {
    for (y= 0; y < 16; ++y)
      for (x = 0; x < 16; ++x)
      {
        SetChar(i, x+1, y+1);
        ++i;
      }
    i += 16;
    
    
    SetTileOrigin(19, 1);
    for (y = 0; y < 8; ++y)
      for (x = 0; x < 8; ++x)
      {
        DrawTileFast(x*y - z, x, y);
        ++z;
      }
    z -= 63;
  while (1)
  {
    Scroll(right);
    //Scroll(right);
    //Scroll(down);
    //Scroll(down);
    //Scroll(left);
    //Scroll(left);
    //Scroll(up);
    //Scroll(up);
    //DrawBorder("Hello C64!@", 0, 0, COLS, 3, false);
    
  }
  }
}
