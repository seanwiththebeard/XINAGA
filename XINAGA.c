#include "xinaga.h"

//#link "Graphics.c"


void main()
{
  byte x, y, z;
  byte i = 0;
  InitializeGraphics();  

  DrawBorder("Character Set@", 0, 0, 18, 18, false);
  DrawBorder("Tiles@", 18, 0, 18, 18, false);
  #if defined(__APPLE2__)
  DrawBorder("Hello Apple!@", 0, ROWS - 3, 16, 2, false);
  #endif
  #if defined(__C64__)
  DrawBorder("Hello C64!@", 0, ROWS - 3, 16, 3, false);
  #endif
  while (1)
  {
    
    for (y= 0; y < 16; ++y)
      for (x = 0; x < 16; ++x)
      {
        SetCharBuffer(i, x+1, y+1);
        ++i;
      }    
    CopyBufferArea(1, 1, 16, 16);
    //CopyBuffer();
    i += 16;
    
    z = i;
    SetTileOrigin(19, 1);
    for (y = 8; y != 0; --y)
      for (x = 8; x != 0; --x)
      {
        DrawTileFast(z, x - 1, y - 1);
        ++z;
      }
    
    
    //ClearScreen();
  }
}
