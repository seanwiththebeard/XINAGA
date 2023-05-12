#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

#define height 16
#define width 16
#define posX 13
#define posY 6
#define points 32
byte map[height][width];

void GenerateMap()
{
  byte x, y;
  #define grass 0x88
  #define water 0x84
  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      map[y][x] = water;
    }
  srand(randseed);
  for (x = 0; x < points; ++x)
  {
    map[rand() % height][rand() % width] = grass;
  }
  
  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      SetChar(map[y][x], posX + x, posY + y);
    }
}

screenName Update_MapGen()
{
  screenName nextScreen = Title;
  bool exit = false;
  DrawBorder("Map Generator@",posX - 1, posY - 1, width + 2, height + 2, true);
  GenerateMap();
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
      {
        ++randseed;
        srand(randseed);
        GenerateMap();
        //exit = true;
      }
  }
  
  return nextScreen;
}