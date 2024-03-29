#include "xinaga.h"
#include "System_MessageWindow.h"

//Tests
void DrawCharset()
{
  byte x, y;
  byte i = 0;
  DrawBorder("Character Set@", 0, 0, 18, 18, false);
  
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
  #if defined(__APPLE2__)
  #define sizeX 8
  #define sizeY 8
  #endif
  #if defined(__C64__)
  #define sizeX 9
  #define sizeY 9
  #endif
  byte MapData[sizeX * sizeY];
  byte MapTemp[sizeX * sizeY];
  byte charX, charY, charI;
  byte x, y, z, i = 0;
  DrawBorder("Map@", 0, 0, sizeX * 2 + 2, sizeY * 2 + 2, false);
  for (x = 0; x < (sizeX * sizeY); ++x)
    MapData[x] = 36;
  MapData[36] = 47;
  MapData[26] = 44;
  //MapData[10] = 2;
  SetTileOrigin(1, 1);
  
  charX = 2;
  charY = 0;
  charI = 2;
  while(1)
  {
    memcpy(&MapTemp[0], &MapData[0], sizeX * sizeY);
    MapTemp[charY*sizeX + charX] = charI;
    if (i & 1)
      charX++;
    else
      charY++;
    ++i;
    
    if (charX > sizeX - 1)
      charX -= sizeX;
    if (charY > sizeY - 1)
      charY -= sizeY;
    
    z = 0;
    StoreBuffer();
    for (y = 0; y < sizeY; ++y)
      for (x = 0; x < sizeX; ++x)
      {        
        DrawTileFast(MapTemp[z], x, y);
        ++z;
    }
    SwapBuffer();
    
    z = 2;
    for (y = 0; y < sizeY; ++y)
      for (x = 0; x < sizeX; ++x)
      {        
        //MapData[z - 1] = MapData[z];
        ++z;
    }
  }
}

/*void ScrollScreen()
{
    Scroll(right);
    Scroll(right);
    Scroll(down);
    Scroll(down);
    Scroll(left);
    Scroll(left);
    Scroll(up);
    Scroll(up);
}*/

void Demo()
{
  InitializeInput();
  InitializeGraphics();
  ClearScreen();
  
  #if defined(__APPLE2__)
  DrawBorder("Hello Apple!@", 0, ROWS - 3, COLS, 3, false);
  #endif
  
  #if defined(__C64__)
  DrawBorder("Hello C64!@", 0, ROWS - 3, COLS, 3, false);
  #endif
  
  //DrawCharset();
  //DrawTiles();
  //SetTileOrigin(3, 2);
  
  LoadMap();
  BlankMessageWindow();
  DrawMessageWindow();
  MapUpdate();
  while(1)
  {
    WriteLineMessageWindow("Hello@", 0);
    WriteLineMessageWindow("Hello@", 1);
    
    //DiskSave("data1", 0x2001, 0x2000);
    //DiskLoad("data1", 0x2000);
    //DrawCharset();
    //SwapBuffer();
    //ClearScreen();
  }
}
