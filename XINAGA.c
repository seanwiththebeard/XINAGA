#include "xinaga.h"
#include "Common.h"

//#link "Graphics.c"
//#link "Common.c"
#if defined(__APPLE2__)
//#define CFGFILE apple2-hgr2.cfg
//#resource "apple2-hgr2.cfg"
#endif

//Tests
void DrawCharset()
{
  byte x, y;
  byte i = 0;
  DrawBorder("Character Set@", 0, 0, 18, 18, false);
  #if defined(__APPLE2__)
  DrawBorder("Hello Apple!@", 0, ROWS - 3, COLS, 2, false);
  #endif
  #if defined(__C64__)
  DrawBorder("Hello C64!@", 0, ROWS - 3, COLS, 3, false);
  #endif
  
  for (y= 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
    {
      SetChar(i, x+1, y+1);
      ++i;
    }
  i += 16;
}

void DrawTiles()
{
  byte x, y, z;
  DrawBorder("Tiles@", 18, 0, 18, 18, false);
  SetTileOrigin(19, 1);
  StoreBuffer();
  for (y = 0; y < 8; ++y)
    for (x = 0; x < 8; ++x)
    {
      DrawTileFast(/*x*y - */z, x, y);
      ++z;
    }
    SwapBuffer();
}

void DrawMap()
{
  #define sizeX 9
  #define sizeY 9
  byte MapData[sizeX * sizeY];
  byte x, y, z;
  DrawBorder("Map@", 0, 0, sizeX * 2 + 2, sizeY * 2 + 2, false);
  for (x = 0; x < (sizeX * sizeY); ++x)
    MapData[x] = 36;
  MapData[36] = 47;
  MapData[26] = 44;
  MapData[10] = 2;
  SetTileOrigin(1, 1);
  
  while(1)
  {
    bgcolor(0);
    z = 0;
    StoreBuffer();
    for (y = 0; y < sizeY; ++y)
      for (x = 0; x < sizeX; ++x)
      {        
        DrawTileFast(MapData[z], x, y);
        ++z;
    }
    SwapBuffer();
    
    z = 2;
    for (y = 0; y < sizeY; ++y)
      for (x = 0; x < sizeX; ++x)
      {        
        MapData[z - 1] = MapData[z];
        ++z;
    }
  }
}

void ScrollScreen()
{
    Scroll(right);
    Scroll(right);
    Scroll(down);
    Scroll(down);
    Scroll(left);
    Scroll(left);
    Scroll(up);
    Scroll(up);
}

void main()
{
  InitializeGraphics();
  while(1)
  {
    //ClearScreen();
    DrawCharset();
    //DrawTiles();
    //InvertScreen();
    InvertScreen();
    //InvertCharacterSet();
    //DrawMap();
    //ScrollScreen();
  }
}
