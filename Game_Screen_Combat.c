#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

byte fillTile = 36;
bool CombatSuccess = false;
bool exitCombat = false;
int SelectedCharacter;
int SelectedTarget;
int MovementRemaining = 0;
#define CombatMapWidth 8
#define CombatMapHeight 8

#define MaxCombatParticipants CombatMapWidth * CombatMapHeight


#define consolePosX 1
#define consolePosY 18
#define consoleWidth 29
#define consoleHeight 5
#define consoleDelay 1

//Functions
void Initialize(void);
void ClearRoster(void);
void GetCharacters(void);
void GetMonsters(void);
void RollInitiative(void);

void DoCombatRound(void);
void GetActionSelection(void);
void GetTargetSelection(void);

//Actions
void SelectPlayerAction(void);
void SelectMonsterAction(void);
void SelectionAttackTargetPhysical(void);
void SelectionAttackTargetSpell();
void SelectionUseItem(void);
void SelectionMoveCharacter(void);
byte failedWander = 0;

void MoveCombatCharacter(byte index, byte direction);
void PhysicalAttack(void);
void SpellAttack(void);
void ApplyCondition(void);
void SetCharacterDead(void);

//Drawing
void DrawCombatMap(void);
void DrawCharacters(void);
void DrawOneCharacter(void);
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
  byte targetIndex;
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
  SelectedCharacter = 0;
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
  DrawCharStats();
  DrawCombatMap();

  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
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
    combatParticipant[i].movement = getPartyMember(i)->DEX / 2;
    ++SelectedCharacter;
  }
}

void GetMonsters(void)
{
  byte i;
  byte MonsterCount = 8;
  byte LastMonster = SelectedCharacter + MonsterCount;
  //byte c = CountParty();
  //byte offset;
  //while (MonsterCount > (MaxCombatParticipants - c))
  // --MonsterCount;
  //offset = MaxCombatParticipants - MonsterCount;

  for (i = SelectedCharacter; i < LastMonster; ++i)
    //for (i = 0; i < MonsterCount; ++i)
  {    
    combatParticipant[i].isPlayerChar = false;
    combatParticipant[i].tileIndex = 33 + i;
    combatParticipant[i].posX = i;
    while (combatParticipant[i].posX >= CombatMapWidth)
      combatParticipant[i].posX -= CombatMapWidth;
    combatParticipant[i].posY = 2 + (i / CombatMapWidth);
    combatParticipant[i].initiativeMod = 0;
    combatParticipant[i].active = true;
    combatParticipant[i].alive = true;
    combatParticipant[i].movement = 4;
    combatParticipant[i].targetIndex = rand() % CountParty();
    ++SelectedCharacter;
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

void WriteRemainingMovement()
{
  if (MovementRemaining > 0)
    sprintf(strTemp, "MovementLeft: %d of %d@", MovementRemaining, combatParticipant[SelectedCharacter].movement);
  else
    sprintf(strTemp, "Moved@");
  //WriteLineMessageWindow("@", 0);  
  SetLineMessageWindow(strTemp, 0);
}

void SelectionMoveCharacter(void)
{
  //byte flashTileIndex = combatParticipant[SelectedCharacter].tileIndex;
  MovementRemaining = combatParticipant[SelectedCharacter].movement;
  //WriteLineMessageWindow("@", 0);
  WriteRemainingMovement();
  DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  while(MovementRemaining > 0)
  {
    byte remaining = MovementRemaining;
    //if(combatParticipant[SelectedCharacter].tileIndex == flashTileIndex)
    //combatParticipant[SelectedCharacter].tileIndex = fillTile;
    //else
    //combatParticipant[SelectedCharacter].tileIndex = flashTileIndex;
    //DrawOneCharacter();
    //wait_vblank(10);
    UpdateInput();
    if (InputChanged())
    {
      ClearArrow();
      if (InputUp())
        MoveCombatCharacter(SelectedCharacter, up);
      if (InputDown())
        MoveCombatCharacter(SelectedCharacter, down);
      if (InputLeft())
        MoveCombatCharacter(SelectedCharacter, left);
      if (InputRight())
        MoveCombatCharacter(SelectedCharacter, right);
      if (InputFire())
        MovementRemaining = 0;
      DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
    }
    if (remaining  != MovementRemaining)
      WriteRemainingMovement();
  }
  //combatParticipant[SelectedCharacter].tileIndex = flashTileIndex;
  DrawOneCharacter();
  ClearArrow();
}

bool SelectNextCharacter()
{
  bool found = false;
  byte count = 0;
  while (!found)
  {
    ++SelectedCharacter;
    if (SelectedCharacter >= MaxCombatParticipants)
      SelectedCharacter = 0;

    //if (combatParticipant[SelectedCharacter].isPlayerChar)   
    if (combatParticipant[SelectedCharacter].active)
      if (combatParticipant[SelectedCharacter].alive)
        found = true;

    ++count;
    if (count > MaxCombatParticipants)
    {
      sprintf(strTemp, "No Entities@");
      WriteLineMessageWindow(strTemp, consoleDelay);
      return false; 
    }
  }
  return true;
}

void DoCombatRound()
{
  RollInitiative();

  if (SelectNextCharacter())
    GetActionSelection();
  else
  {
    exitCombat = true;
  }
}

void GetActionSelection(void)
{
  ReadyArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  if(combatParticipant[SelectedCharacter].isPlayerChar)
    SelectPlayerAction();
  else
    SelectMonsterAction();
}

void GetTargetSelection(void)
{
  int x = combatParticipant[SelectedCharacter].posX;
  int y = combatParticipant[SelectedCharacter].posY;
  byte i;
  
  ClearArrow();
  DrawArrow(x, y);

  UpdateInput();
  while (InputFire())
  {
    UpdateInput();
  }
  while (!InputFire())
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
        --y;
      if (InputDown())
        ++y;
      if (InputLeft())
        --x;
      if (InputRight())
        ++x;
    

    while (x < 0)
      ++x;
    while (x >= CombatMapWidth)
      --x;
    while (y < 0)
      ++y;
    while (y >= CombatMapHeight)
      --y;

    while (x < combatParticipant[SelectedCharacter].posX - 1)
      ++x;
    while (x > combatParticipant[SelectedCharacter].posX + 1)
      --x;
    while (y < combatParticipant[SelectedCharacter].posY - 1)
      ++y;
    while (y > combatParticipant[SelectedCharacter].posY + 1)
      --y;
      
      ClearArrow();
      DrawArrow(x, y);

    SelectedTarget = -1;
    for (i = 0; i < MaxCombatParticipants; ++i)
    {
      if (combatParticipant[i].posX == x)
        if(combatParticipant[i].posY == y)
        {
          SelectedTarget = i;
          i = MaxCombatParticipants;
        }
    }
    }
  }
  ClearArrow();
}

void MonsterWander()
{
  failedWander = 0;
  MovementRemaining = combatParticipant[SelectedCharacter].movement;
  //DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  while(MovementRemaining > 0)
  {
    byte remaining = MovementRemaining;
    WriteRemainingMovement();
    ClearArrow();
    switch (rand() % 4)
    {
      case 0:
        MoveCombatCharacter(SelectedCharacter, up);
        break;
      case 1:
        MoveCombatCharacter(SelectedCharacter, down);
        break;
      case 2:
        MoveCombatCharacter(SelectedCharacter, left);
        break;
      case 3:
        MoveCombatCharacter(SelectedCharacter, right);
        break;
      default:
        break;
    }
    DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
    if(remaining == MovementRemaining)
      ++failedWander;
    if (failedWander >= 3)
    {
      MovementRemaining = 0;
      sprintf(strTemp, "Wander Failed M#%d@", SelectedCharacter);
      WriteLineMessageWindow(strTemp, 0);
    }
  }
  ClearArrow();
  WriteRemainingMovement();
}

//Actions
void SelectMonsterAction(void)
{
  SelectPlayerAction();
  //sprintf(strTemp, "Monster Action %d@", SelectedCharacter);
  //WriteLineMessageWindow(strTemp, consoleDelay);
  //MonsterWander();
}
void SelectPlayerAction(void)
{
  bool finished = false;
  byte moveX = combatParticipant[SelectedCharacter].posX;
  byte moveY = combatParticipant[SelectedCharacter].posY;
  byte tempTile;
  
  DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  ResetMenu("PLAYER@",31, 18, 8, 5, 5);
  SetMenuItem(0, "Move@");
  SetMenuItem(1, "Attack@");
  SetMenuItem(2, "Magic@");
  SetMenuItem(3, "Item@");
  SetMenuItem(4, "End@");
  
  while (!finished)
  switch (GetMenuSelection())
  {
    case 0:
      tempTile = combatParticipant[SelectedCharacter].tileIndex;
      combatParticipant[SelectedCharacter].tileIndex = fillTile;
      DrawOneCharacter();
      ClearArrow();
      combatParticipant[SelectedCharacter].posX = moveX;
      combatParticipant[SelectedCharacter].posY = moveY;
      combatParticipant[SelectedCharacter].tileIndex = tempTile;
      DrawOneCharacter();
      SelectionMoveCharacter();
      break;
    case 1:
      GetTargetSelection();
      finished = true;
      break;
    case 2:
      finished = true;
      break;
    case 3:
      finished = true;
      break;
    case 4:
      finished = true;
    default:
      break;
  }
}

void SelectionAttackTargetPhysical(void);
void SelectionAttackTargetSpell();
void SelectionUseItem(void);
void SelectionMoveCharacter(void);


bool CheckCombatMapCollision(byte direction)
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

  switch(direction)
  {
    case up:
      if(combatParticipant[SelectedCharacter].posY <= 0)
        combatParticipant[SelectedCharacter].active = false;
      break;
    case down:
      if(combatParticipant[SelectedCharacter].posY >= CombatMapHeight - 1)
        combatParticipant[SelectedCharacter].active = false;
      break;
    case left:
      if(combatParticipant[SelectedCharacter].posX <= 0)
        combatParticipant[SelectedCharacter].active = false;
      break;
    case right:
      if(combatParticipant[SelectedCharacter].posX >= CombatMapWidth - 1)
        combatParticipant[SelectedCharacter].active = false;
      break;
    default:
      break;
  }
  if (!combatParticipant[SelectedCharacter].active)
  {
    MovementRemaining = 0;
    combatParticipant[SelectedCharacter].tileIndex = fillTile;
    DrawOneCharacter();
  }
  return false;
}
void MoveCombatCharacter(byte index, byte direction)
{
  byte tempTile = combatParticipant[index].tileIndex;
  combatParticipant[index].tileIndex = fillTile;
  DrawOneCharacter();
  if (!CheckCombatMapCollision(direction))
  {
    --MovementRemaining;
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
  }
  combatParticipant[index].tileIndex = tempTile;
  DrawOneCharacter();
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

void DrawOneCharacter()
{
  if (combatParticipant[SelectedCharacter].active)
  {
    wait_vblank(1); //Reduces flicker
    DrawTileDirect(combatParticipant[SelectedCharacter].tileIndex, combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  }
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
  srand(0);

  while (!exitCombat)
  {
    DoCombatRound();
  }

  sprintf(strTemp, "Combat End@");
  WriteLineMessageWindow(strTemp, consoleDelay);
  sprintf(strTemp, "Press Space...@");
  WriteLineMessageWindow(strTemp, consoleDelay);
  while (!InputFire())
  {
    UpdateInput();
  }

  //Malloc free combat data
  return nextScreen;
}