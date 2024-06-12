#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

screenName currentScreen = EditParty;

RaceDescriptionDef RaceDescription[4] = 
{
  {"Human@", 255},
  {"Elf@", 6}, 
  {"Dwarf@", 255}, 
  {"Halfling@", 6}
};


ClassDescriptionDef ClassDescription[4]= 
{
  {"Fighter@", 8},
  {"Wizard@", 4}, 
  {"Cleric@", 6}, 
  {"Thief@", 4},
};

sbyte AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};

void SwitchScreen(screenName screen)
{
  //ScreenDisable();
  ClearScreen();
  //Load specified screen
  //UpdateInput();
  currentScreen = screen;
  //ScreenEnable();
  
  switch (currentScreen)
  {
    case Title:
      currentScreen = Update_Title();
      break;
    //case Credits:
      //currentScreen = Update_Credits();
      //break;
    case EditParty:
      currentScreen = DrawAddCharacterScreen();
      break;
    case Map:
      //LoadMap();
      currentScreen = MapUpdate();
      break;
    case Combat:
      currentScreen = Update_Combat();
      break;
    case MapGen:
      currentScreen = Update_MapGen();
      break;
    case Scenario:
      currentScreen = Update_Scenario();
    default:
      break;
  }
  
  SwitchScreen(currentScreen);
}

//Tests
/*
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
      DrawTileFast(z, x, y);
      ++z;
    }
    SwapBuffer();
}*/

/*void DrawMap()
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
}*/

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
  //byte freq = 0;  
  
  InitializeInput();
  InitializeGraphics();
  ClearScreen();    
  
  #if defined(__APPLE2__)
  //DrawBorder("Hello Apple!@", 0, ROWS - 3, COLS, 3, false);
  #endif
  
  #if defined(__C64__)
  //DrawBorder("Hello C64!@", 0, ROWS - 3, COLS, 3, false);
  #endif
  //DrawCharset();
  //DrawTiles();
  //SetTileOrigin(3, 2);
  
  LoadMap();
  
  SwitchScreen(currentScreen);
  
  while(1)
  {
    //WriteLineMessageWindow("Hello@", 0);
    //PlaySound(2, freq);
    //++freq;
    //WriteLineMessageWindow("Hello@", 1);

    //DiskSave("data1", 0x2001, 0x2000);
    //DiskLoad("data1", 0x2000);
    //DrawCharset();
    //SwapBuffer();
    //ClearScreen();
    //UpdateInput();
    //if (InputFire())
      //break;
  }
}
