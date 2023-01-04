#include "xinaga.h"

//#link "Graphics.c"


void main()
{
  byte x, y, z;
  byte i = 0;
  InitializeGraphics();  
  
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
    PrintString("Hello Apple!@", 16, y, false, false);
    #endif
    #if defined(__C64__)
    PrintString("Hello C64!@", 16, y, false, false);
    #endif
    
    SetTileOrigin(17, 0);
    while (1)
    {
      for (y= 0; y < 8; ++y)
        for (x = 0; x < 9; ++x)
        {
          DrawTileFast(z, x, y);
          ++z;
        }
      
    DrawLineH(0, 0, 0, 18);
      
    }
    //ClearScreen();
  }
}
