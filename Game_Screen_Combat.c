#include "game.h"
#include "xinaga.h"

byte index = 36;

void DrawCombatMap(void);
void DrawCharacters(void);

void DrawCombatMap()
{
  byte x, y = 0;
  SetTileOrigin(1, 1);
  //wait_vblank(1);
  
  StoreBuffer();
  for (y = 0; y < 8; ++y)
    for (x = 0; x < 8; ++x)
    {
      DrawTileFast(index, x, y);
    }
  DrawCharacters();
  SwapBuffer();
}

void DrawCharacters()
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
    DrawTileFast(i, 2+i, 6);
}

screenName Update_Combat()
{
  screenName nextScreen = Map;
  bool exit = false;
  
  ClearScreen();
  DrawBorder("Combat@",0, 0, 18, 18, false);
  DrawCombatMap();
  //DrawCharacters();
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}