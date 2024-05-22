#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

byte fillTile = 36;
bool CombatSuccess = false;
bool exitCombat = false;
#define MaxCombatParticipants 8

//Functions
void Initialize(void);
void ClearRoster(void);
void GetCharacters(void);
void GetMonsters(void);
void RollInitiative(void);

void DoCombatRound(void);
void GetActionSelection(void);

//Actions
void SelectionPlayerSelectAction(void);
void SelectionAttackTargetPhysical(void);
void SelectionAttackTargetSpell();
void SelectionUseItem(void);
void SelectionMoveCharacter(void);

void MoveCharacter(byte index, byte direction);
void PhysicalAttack(void);
void SpellAttack(void);
void ApplyCondition(void);

//Drawing
void DrawCombatMap(void);
void DrawCharacters(void);
void DrawSpellEffect(void);

void ApplyCombatRewards();
//Combat Session Data
//Eventually create a structure containing this data and malloc an instance at combat runtime

typedef struct CombatParticipant
{
  bool isPlayerChar;
  byte tileIndex;
  byte posX;
  byte posY;
  byte initiative;
  bool active;
};

struct CombatParticipant combatParticipant[MaxCombatParticipants];

void ClearRoster(void)
{
  byte i;
    for (i = 0; i < MaxCombatParticipants; ++i)
    {
      combatParticipant[i].isPlayerChar = false;
      combatParticipant[i].tileIndex = 0;
      combatParticipant[i].posX = i;
      combatParticipant[i].posY = 7;
      combatParticipant[i].initiative = 0;
      combatParticipant[i].active = false;
    }
}

void Initialize(void)
{
  exitCombat = false;
  ClearScreen();
  
  //Malloc the combat data
  
  ClearRoster();
  GetCharacters();
  GetMonsters();
  
  DrawBorder("Combat@",0, 0, 18, 18, false);
  DrawCombatMap();
  //DrawCharacters();
}

void GetCharacters(void)
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
  {
    combatParticipant[i].isPlayerChar = true;
    combatParticipant[i].tileIndex = getPartyMember(i)->CLASS;
    combatParticipant[i].posX = 2+i;
    combatParticipant[i].posY = 6;
    combatParticipant[i].initiative = (getPartyMember(i)->DEX) / 4 + rand() % 20;
    combatParticipant[i].active = true;
  }
}

void GetMonsters(void)
{
  byte i;
  #define MonsterCount 4
  byte c = CountParty();
  for (i = c; i < c + MonsterCount; ++i)
  {
    if (i > MaxCombatParticipants)
      break;
    
    combatParticipant[i].isPlayerChar = false;
    combatParticipant[i].tileIndex = 8 + i;
    combatParticipant[i].posX = 2+i;
    combatParticipant[i].posY = 2;
    combatParticipant[i].initiative = rand() % 20;
    combatParticipant[i].active = true;
  }
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
      DrawTileFast(fillTile, x, y);
    }
  DrawCharacters();
  SwapBuffer();
}

void DrawCharacters(void)
{
  byte i;

  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    //if (combatParticipant[i].active)
      DrawTileFast(combatParticipant[i].tileIndex, combatParticipant[i].posX, combatParticipant[i].posY);
  }
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