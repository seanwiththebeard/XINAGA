#include "xinaga.h"

//#link "Graphics.c"


void main()
{
  byte x, y, z;
  byte i = 0;
  InitializeGraphics();  
  DrawBorder(0, 0, 18, 18, false);
  DrawBorder(18, 0, 18, 18, false);
  
  
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
    #if defined(__APPLE2__)
    PrintString("Hello Apple!@", 0, 24, false, false);
    #endif
    #if defined(__C64__)
    PrintString("Hello C64!@", 0, 24, false, false);
    #endif
    
    SetTileOrigin(19, 1);
    while (1)
    {
      for (y= 0; y < 7; ++y)
        for (x = 0; x < 8; ++x)
        {
          DrawTileFast(z, x, y);
          ++z;
        }
      
      
    }
    //ClearScreen();
  }
}
