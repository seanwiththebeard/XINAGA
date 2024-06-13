#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LC")
#endif

#define fillTile 36

sbyte SelectedCharacter = 0;
sbyte SelectedTarget = 0;
sbyte MovementRemaining = 0;
bool CombatSuccess = false;
bool exitCombat = false;

#define CombatMapWidth 11
#define CombatMapHeight 8

#define MaxCombatParticipants 12 //CombatMapWidth * CombatMapHeight
#define MonsterCount 8


#define consolePosX 1
#define consolePosY 18
#define consoleWidth 29
#define consoleHeight 5
#define consoleDelay 1

#define menuPosX consolePosX + consoleWidth + 1
#define menuPosY consolePosY
#define menuWidth 8
#define menuHeight 5

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
//byte failedWander = 0;

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
  byte tileIndex;
  byte targetIndex;
  byte initiative;
  byte movement;
  bool active;
  bool alive;
  bool isPlayerChar;
  sbyte posX;
  sbyte posY;
  sbyte initiativeMod;
};

struct CombatParticipant combatParticipant[MaxCombatParticipants] = {};
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

  DrawBorder("Combat@",0, 0, 2 + 2*CombatMapWidth, 2 + 2*CombatMapHeight, false);
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
    combatParticipant[i].tileIndex = 5; //33+i;
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
  {
    sprintf(strTemp, "Movement Left:%dof%d@", MovementRemaining, combatParticipant[SelectedCharacter].movement);
    SetLineMessageWindow(strTemp, 0);
  }
  else
  {
    if (combatParticipant[SelectedCharacter].isPlayerChar)
      SetLineMessageWindow("Moved, press space@", 0);
    else
      SetLineMessageWindow("Monster moved@", 0);
  }
}

void SelectionMoveCharacter(void)
{
  MovementRemaining = combatParticipant[SelectedCharacter].movement;
  WriteRemainingMovement();
  combatParticipant[SelectedCharacter].active = true;
  DrawOneCharacter();
  DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  while(InputFire())
    UpdateInput();
  while(!InputFire())
  {
    byte remaining = MovementRemaining;
    UpdateInput();
    if (InputChanged())
    {
      ClearArrow();
      if (MovementRemaining > 0)
      {
        if (InputUp())
          MoveCombatCharacter(SelectedCharacter, up);
        if (InputDown())
          MoveCombatCharacter(SelectedCharacter, down);
        if (InputLeft())
          MoveCombatCharacter(SelectedCharacter, left);
        if (InputRight())
          MoveCombatCharacter(SelectedCharacter, right);
      }

      if(combatParticipant[SelectedCharacter].active)
        DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
      if (remaining  != MovementRemaining)
        WriteRemainingMovement();
    }

  }
  DrawOneCharacter();
  SetLineMessageWindow("@",0);
}

bool SelectNextCharacter()
{
  bool found = false;
  byte count = 0;
  TickMoonPhase();
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
      WriteLineMessageWindow("No Entities@", consoleDelay);
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
  byte i;
  sbyte x = combatParticipant[SelectedCharacter].posX;
  sbyte y = combatParticipant[SelectedCharacter].posY;
  SetLineMessageWindow("Select Target@",0);

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
  byte failedWander = 0;
  MovementRemaining = combatParticipant[SelectedCharacter].movement;
  //DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
  while(MovementRemaining > 0)
  {
    byte remaining = MovementRemaining;
    WriteRemainingMovement();
    ClearArrow();
    srand (rand() % 32767);
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
      SetLineMessageWindow(strTemp, 0);
    }
  }
  ClearArrow();
  WriteRemainingMovement();
}

//Actions
void SelectMonsterAction(void)
{
  //SelectPlayerAction();
  //sprintf(strTemp, "Monster Action %d@", SelectedCharacter);
  //WriteLineMessageWindow(strTemp, consoleDelay);
  MonsterWander();
}
void SelectPlayerAction(void)
{
  byte tempTile;
  bool finished = false;
  byte moveX = combatParticipant[SelectedCharacter].posX;
  byte moveY = combatParticipant[SelectedCharacter].posY;

  ResetMenu("Action@", menuPosX, menuPosY, menuWidth, menuHeight, 5);
  SetMenuItem(0, "Move@");
  SetMenuItem(1, "Attack@");
  SetMenuItem(2, "Magic@");
  SetMenuItem(3, "Item@");
  SetMenuItem(4, "End@");

  while (!finished)
  {
    SetLineMessageWindow("Command?@",0);
    if(combatParticipant[SelectedCharacter].active)
      DrawArrow(combatParticipant[SelectedCharacter].posX, combatParticipant[SelectedCharacter].posY);
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
        MovementRemaining = combatParticipant[SelectedCharacter].movement;
        SelectionMoveCharacter();
        break;
      case 1:
        if (combatParticipant[SelectedCharacter].active)
        {
          GetTargetSelection();
          finished = true;
        }
        break;
      case 2:
        if (combatParticipant[SelectedCharacter].active)
        {
          finished = true;
        }
        break;
      case 3:
        if (combatParticipant[SelectedCharacter].active)
        {
          finished = true;
        }
        break;
      case 4:
        finished = true;
      default:
        break;
    }
    ClearArrow();
  }
}

void SelectionAttackTargetPhysical(void);
void SelectionAttackTargetSpell();
void SelectionUseItem(void);
void SelectionMoveCharacter(void);


bool CheckCombatMapCollision(byte dir)
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    if (i != SelectedCharacter)
      if (combatParticipant[i].active)
        if (combatParticipant[i].alive)
          switch(dir)
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

  switch(dir) //Deactivate if off map
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

void MoveCombatCharacter(byte index, byte dir)
{
  byte tempTile = combatParticipant[index].tileIndex;
  combatParticipant[index].tileIndex = fillTile;
  DrawOneCharacter();
  if (!CheckCombatMapCollision(dir))
  {
    --MovementRemaining;
    switch(dir)
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
  byte x;
  byte y;
  SetTileOrigin(1, 1);
  //wait_vblank(1);

  //StoreBuffer();
  for (y = 0; y < CombatMapHeight; ++y)
    for (x = 0; x < CombatMapWidth; ++x)
    {
      DrawTileIndex = fillTile;
      DrawTileX = x;
      DrawTileY = y;
      DrawTileDirect();
    }
  DrawCharacters();
  //SwapBuffer();
}

void DrawOneCharacter()
{
  if (combatParticipant[SelectedCharacter].active)
  {
    wait_vblank(1); //Reduces flicker
    DrawTileIndex = combatParticipant[SelectedCharacter].tileIndex;
    DrawTileX = combatParticipant[SelectedCharacter].posX;
    DrawTileY = combatParticipant[SelectedCharacter].posY;
    DrawTileDirect();
  }
}

void DrawCharacters(void)
{
  byte i;

  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    if (combatParticipant[i].active)
    {
      DrawTileIndex = combatParticipant[i].tileIndex;
      DrawTileX = combatParticipant[i].posX;
      DrawTileY = combatParticipant[i].posY;
      DrawTileDirect();
    }
  }
}

screenName Update_Combat(void)
{
  Initialize();
  //randseed = 0;
  //srand(randseed);

  while (!exitCombat)
  {
    DoCombatRound();
  }
  WriteLineMessageWindow("Combat End, Press Space...@", consoleDelay);
  while (!InputFire())
  {
    UpdateInput();
  }

  //Malloc free combat data
  return Map;
}