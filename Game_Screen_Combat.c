#include "game.h"
#include "xinaga.h"

byte index = 36;
bool CombatSuccess = false;
bool exitCombat = false;

//Functions
void Initialize(void);
void GetCharacters(void);
void GetMonsters(void);
void RollInitiative(void);

void DoCombatRound(void);

void PlayerSelectAction(void);

void PhysicalAttack(void);
void SpellAttack(void);
void ApplyCondition(void);

void DrawCombatMap(void);
void DrawCharacters(void);
void DrawSpellEffect(void);

//Combat Session Data
//Eventually create a structure containing this data and malloc an instance at combat runtime


void ApplyCombatRewards();

void Initialize(void)
{
  exitCombat = false;
  ClearScreen();
  
  //Malloc the combat data
  
  GetCharacters();
  GetMonsters();
  
  DrawBorder("Combat@",0, 0, 18, 18, false);
  DrawCombatMap();
  //DrawCharacters();
}

void GetCharacters(void)
{
}

void GetMonsters(void)
{
}

void RollInitiative(void)
{
}

void DoCombatRound()
{
  RollInitiative();
  
}

void DrawCombatMap(void)
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

void DrawCharacters(void)
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
    DrawTileFast(i, 2+i, 6);
}

screenName Update_Combat(void)
{
  screenName nextScreen = Map;
  
  Initialize();
  
  while (!exitCombat)
  {
    DoCombatRound();
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exitCombat = true;
  }
  
 //Malloc free combat data
  return nextScreen;
}