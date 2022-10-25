#include "xinaga.h"

//#link "Graphics.c"


void main()
{
  byte x, y;
  byte i = 0;
  InitializeGraphics();  
  while (1)
  {
    for (y= 0; y < 16; ++y)
      for (x = 0; x < 16; ++x)
      {
        SetCharBuffer(i, x, y);
        ++i;
      }    
    CopyBufferArea(0, 0, 16, 16);
    //CopyBuffer();
    i += 16;
    #if defined(__APPLE2__)
    PrintString("Hello Apple!@", 16, y, false, false);
    #endif
    #if defined(__C64__)
    PrintString("Hello C64!@", 16, y, false, false);
    #endif
    ClearScreen();
  }
}
