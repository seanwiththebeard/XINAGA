#include "xinaga.h"

//#link "Graphics.c"


void main()
{
  byte x, y;
  byte i = 0;
  InitializeGraphics();
  
  PrintString("Hello World!@", 16, 16, true, false);
  
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
  }
}
