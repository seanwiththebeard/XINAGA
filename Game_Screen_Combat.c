#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

byte fillTile = 36;
bool CombatSuccess = false;
bool exitCombat = false;
byte SelectedCharacter = 9;
byte MovementRemaining = 0;
#define MaxCombatParticipants 12
#define CombatMapWidth 8
#define CombatMapHeight 8

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

void MoveCombatCharacter(byte index, byte direction);
void PhysicalAttack(void);
void SpellAttack(void);
void ApplyCondition(void);
void SetCharacterDead(void);

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
  int posX;
  int posY;
  byte initiative;
  int initiativeMod;
  byte movement;
  bool active;
  bool alive;
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
    combatParticipant[i].alive = false;
    combatParticipant[i].movement = 0;  
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
    combatParticipant[i].initiative = 0;
    combatParticipant[i].initiativeMod = (getPartyMember(i)->DEX - 10) / 2;
    combatParticipant[i].active = true;
    combatParticipant[i].alive = true;
  }
}

void GetMonsters(void)
{
  byte i;
  byte MonsterCount = 4;
  byte c = CountParty();
  byte offset;
  while (MonsterCount > (MaxCombatParticipants - c))
    --MonsterCount;
  offset = MaxCombatParticipants - MonsterCount;

  for (i = (MaxCombatParticipants - MonsterCount); i < MaxCombatParticipants; ++i)
  {    
    combatParticipant[i].isPlayerChar = false;
    combatParticipant[i].tileIndex = 8 + i;
    combatParticipant[i].posX = 2 + i - offset;
    combatParticipant[i].posY = 2;
    combatParticipant[i].initiativeMod = 0;
    combatParticipant[i].active = true;
    combatParticipant[i].alive = true;
  }
}

void RollInitiative(void)
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    combatParticipant[i].initiative = rand() % 20 + combatParticipant[i].initiativeMod;
  }
}

void DoCombatRound()
{
  RollInitiative();

  UpdateInput();
  if (InputChanged())
  {
    if (InputUp())
      MoveCombatCharacter(SelectedCharacter, up);
    if (InputDown())
      MoveCombatCharacter(SelectedCharacter, down);
    if (InputLeft())
      MoveCombatCharacter(SelectedCharacter, left);
    if (InputRight())
      MoveCombatCharacter(SelectedCharacter, right);
    if (InputFire())
      exitCombat = true;
  }
}

void GetActionSelection(void)
{
}

//Actions
void SelectionPlayerSelectAction(void);
void SelectionAttackTargetPhysical(void);
void SelectionAttackTargetSpell();
void SelectionUseItem(void);
void SelectionMoveCharacter(void);


bool CheckCombatMapCollection(byte direction)
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    if (i != SelectedCharacter)
      if (combatParticipant[i].active)
        if (combatParticipant[i].alive)
          switch(direction)
          {
            case up:
              if(combatParticipant[SelectedCharacter].posY - 1 == combatParticipant[i].posY)

                if (combatParticipant[SelectedCharacter].posX == combatParticipant[i].posX)
                  return true;
              break;
            case down:
              if(combatParticipant[SelectedCharacter].posY + 1 == combatParticipant[i].posY)

                if (combatParticipant[SelectedCharacter].posX == combatParticipant[i].posX)
                  return true;
              break;
            case left:
              if (combatParticipant[SelectedCharacter].posX - 1 == combatParticipant[i].posX)
                if(combatParticipant[SelectedCharacter].posY == combatParticipant[i].posY)
                  return true;
              break;
            case right:
              if (combatParticipant[SelectedCharacter].posX + 1 == combatParticipant[i].posX)
                if(combatParticipant[SelectedCharacter].posY == combatParticipant[i].posY)
                  return true;
              break;
            default:
              break;
          }
  }
  return false;
}
void MoveCombatCharacter(byte index, byte direction)
{
  if (!CheckCombatMapCollection(direction))
  {
    switch(direction)
    {
      case up:
        --combatParticipant[index].posY;
        break;
      case down:
        ++combatParticipant[index].posY;
        break;
      case left:
        --combatParticipant[index].posX;
        break;
      case right:
        ++combatParticipant[index].posX;
        break;
      default:
        break;
    }

    while (combatParticipant[index].posY >= CombatMapHeight)
      --combatParticipant[index].posY;
    while (combatParticipant[index].posY < 0)
      ++combatParticipant[index].posY;
    while (combatParticipant[index].posX >= CombatMapWidth)
      --combatParticipant[index].posX;
    while (combatParticipant[index].posX < 0)
      ++combatParticipant[index].posX;
    DrawCombatMap();
  }
}
void PhysicalAttack(void);
void SpellAttack(void);
void ApplyCondition(void);

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
    if (combatParticipant[i].active)
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
  }

  //Malloc free combat data
  return nextScreen;
}