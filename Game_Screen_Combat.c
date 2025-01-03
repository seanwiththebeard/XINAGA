#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LC")
#endif

#if defined (__NES__)
#pragma code-name (push, "COMBAT")
#pragma rodata-name (push, "COMBAT")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#define fillTile 36

sbyte SelectedCharacter = 0;
sbyte SelectedTarget = 0;
sbyte MovementRemaining = 0;
bool CombatSuccess = false;
bool exitCombat = false;

#define MaxCombatParticipants 12
#define MonsterCount 1

#define consoleDelay 1

#define CombatMapWidth viewportWidth
#define CombatMapHeight viewportHeight

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

struct 
{
  byte tileIndex[MaxCombatParticipants];
  byte targetIndex[MaxCombatParticipants];
  byte initiative[MaxCombatParticipants];
  byte movement[MaxCombatParticipants];
  bool active[MaxCombatParticipants];
  bool alive[MaxCombatParticipants];
  bool isPlayerChar[MaxCombatParticipants];
  sbyte posX[MaxCombatParticipants];
  sbyte posY[MaxCombatParticipants];
  sbyte initiativeMod[MaxCombatParticipants];
}combatParticipant = {};

void ClearRoster(void)
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    combatParticipant.isPlayerChar[i] = false;
    combatParticipant.tileIndex[i] = 0;
    combatParticipant.posX[i] = i;
    combatParticipant.posY[i] = 7;
    combatParticipant.initiative[i] = 0;
    combatParticipant.active[i] = false;
    combatParticipant.alive[i] = false;
    combatParticipant.movement[i] = 0;  
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

  DrawBorder("Combat@",viewportPosX - 1, viewportPosY - 1, 2 + 2*CombatMapWidth, 2 + 2*CombatMapHeight, true);
  DrawCharStats();
  DrawCombatMap();

  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
}


void GetCharacters(void)
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
  {
    combatParticipant.isPlayerChar[i] = true;
    combatParticipant.tileIndex[i] = getPartyMember(i)->CLASS;
    combatParticipant.posX[i] = 2+i;
    combatParticipant.posY[i] = 6;
    combatParticipant.initiative[i] = 0;
    combatParticipant.initiativeMod[i] = (getPartyMember(i)->DEX - 10) / 2;
    combatParticipant.active[i] = true;
    combatParticipant.alive[i] = true;
    combatParticipant.movement[i] = getPartyMember(i)->DEX / 2;
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
    combatParticipant.isPlayerChar[i] = false;
    combatParticipant.tileIndex[i] = 5; //33+i;
    combatParticipant.posX[i] = i;
    while (combatParticipant.posX[i] >= CombatMapWidth)
      combatParticipant.posX[i] -= CombatMapWidth;
    combatParticipant.posY[i] = 2 + (i / CombatMapWidth);
    combatParticipant.initiativeMod[i] = 0;
    combatParticipant.active[i] = true;
    combatParticipant.alive[i] = true;
    combatParticipant.movement[i] = 4;
    combatParticipant.targetIndex[i] = rand() % CountParty();
    ++SelectedCharacter;
  }
}

void RollInitiative(void)
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    combatParticipant.initiative[i] = rand() % 20 + combatParticipant.initiativeMod[i];
  }
}

void WriteRemainingMovement()
{
  if (MovementRemaining > 0)
  {
    sprintf(strTemp, "Movement Left:%dof%d@", MovementRemaining, combatParticipant.movement[SelectedCharacter]);
    SetLineMessageWindow(strTemp, 0);
  }
  else
  {
    if (combatParticipant.isPlayerChar[SelectedCharacter])
      SetLineMessageWindow("Moved, press space@", 0);
    else
      SetLineMessageWindow("Monster moved@", 0);
  }
}

void SelectionMoveCharacter(void)
{
  MovementRemaining = combatParticipant.movement[SelectedCharacter];
  WriteRemainingMovement();
  combatParticipant.active[SelectedCharacter] = true;
  DrawOneCharacter();
  DrawArrow(combatParticipant.posX[SelectedCharacter], combatParticipant.posY[SelectedCharacter]);
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

      if(combatParticipant.active[SelectedCharacter])
        DrawArrow(combatParticipant.posX[SelectedCharacter], combatParticipant.posY[SelectedCharacter]);
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
    if (combatParticipant.active[SelectedCharacter])
      if (combatParticipant.alive[SelectedCharacter])
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
  ReadyArrow(combatParticipant.posX[SelectedCharacter], combatParticipant.posY[SelectedCharacter]);
  if(combatParticipant.isPlayerChar[SelectedCharacter])
    SelectPlayerAction();
  else
    SelectMonsterAction();
}

void GetTargetSelection(void)
{
  byte i;
  sbyte x = combatParticipant.posX[SelectedCharacter];
  sbyte y = combatParticipant.posY[SelectedCharacter];
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

      while (x < combatParticipant.posX[SelectedCharacter] - 1)
        ++x;
      while (x > combatParticipant.posX[SelectedCharacter] + 1)
        --x;
      while (y < combatParticipant.posY[SelectedCharacter] - 1)
        ++y;
      while (y > combatParticipant.posY[SelectedCharacter] + 1)
        --y;

      ClearArrow();
      DrawArrow(x, y);

      SelectedTarget = -1;
      for (i = 0; i < MaxCombatParticipants; ++i)
      {
        if (combatParticipant.posX[i] == x)
          if(combatParticipant.posY[i] == y)
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
  MovementRemaining = combatParticipant.movement[SelectedCharacter];
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
    DrawArrow(combatParticipant.posX[SelectedCharacter], combatParticipant.posY[SelectedCharacter]);
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
  byte moveX = combatParticipant.posX[SelectedCharacter];
  byte moveY = combatParticipant.posY[SelectedCharacter];

  ResetMenu("Action@", selectionPosX, selectionPosY, selectionWidth, selectionHeight, 5);
  SetMenuItem(0, "Move@");
  SetMenuItem(1, "Attack@");
  SetMenuItem(2, "Magic@");
  SetMenuItem(3, "Item@");
  SetMenuItem(4, "End@");

  while (!finished)
  {
    SetLineMessageWindow("Command?@",0);
    if(combatParticipant.active[SelectedCharacter])
      DrawArrow(combatParticipant.posX[SelectedCharacter], combatParticipant.posY[SelectedCharacter]);
    switch (GetMenuSelection())
    {
      case 0:
        tempTile = combatParticipant.tileIndex[SelectedCharacter];
        combatParticipant.tileIndex[SelectedCharacter] = fillTile;
        DrawOneCharacter();
        ClearArrow();
        combatParticipant.posX[SelectedCharacter] = moveX;
        combatParticipant.posY[SelectedCharacter] = moveY;
        combatParticipant.tileIndex[SelectedCharacter] = tempTile;
        DrawOneCharacter();
        MovementRemaining = combatParticipant.movement[SelectedCharacter];
        SelectionMoveCharacter();
        break;
      case 1:
        if (combatParticipant.active[SelectedCharacter])
        {
          GetTargetSelection();
          finished = true;
        }
        break;
      case 2:
        if (combatParticipant.active[SelectedCharacter])
        {
          finished = true;
        }
        break;
      case 3:
        if (combatParticipant.active[SelectedCharacter])
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
      if (combatParticipant.active[i])
        if (combatParticipant.alive[i])
          switch(dir)
          {
            case up:
              if(combatParticipant.posY[SelectedCharacter] - 1 == combatParticipant.posY[i])
                if (combatParticipant.posX[SelectedCharacter] == combatParticipant.posX[i])
                  return true;
              break;
            case down:
              if(combatParticipant.posY[SelectedCharacter] + 1 == combatParticipant.posY[i])
                if (combatParticipant.posX[SelectedCharacter] == combatParticipant.posX[i])
                  return true;
              break;
            case left:
              if (combatParticipant.posX[SelectedCharacter] - 1 == combatParticipant.posX[i])
                if(combatParticipant.posY[SelectedCharacter] == combatParticipant.posY[i])
                  return true;
              break;
            case right:
              if (combatParticipant.posX[SelectedCharacter] + 1 == combatParticipant.posX[i])
                if(combatParticipant.posY[SelectedCharacter] == combatParticipant.posY[i])
                  return true;
              break;
            default:
              break;
          }
  }

  switch(dir) //Deactivate if off map
  {
    case up:
      if(combatParticipant.posY[SelectedCharacter] <= 0)
        combatParticipant.active[SelectedCharacter] = false;
      break;
    case down:
      if(combatParticipant.posY[SelectedCharacter] >= CombatMapHeight - 1)
        combatParticipant.active[SelectedCharacter] = false;
      break;
    case left:
      if(combatParticipant.posX[SelectedCharacter] <= 0)
        combatParticipant.active[SelectedCharacter] = false;
      break;
    case right:
      if(combatParticipant.posX[SelectedCharacter] >= CombatMapWidth - 1)
        combatParticipant.active[SelectedCharacter] = false;
      break;
    default:
      break;
  }
  if (!combatParticipant.active[SelectedCharacter])
  {
    MovementRemaining = 0;
    combatParticipant.tileIndex[SelectedCharacter] = fillTile;
    DrawOneCharacter();
  }
  return false;
}

void MoveCombatCharacter(byte index, byte dir)
{
  byte tempTile = combatParticipant.tileIndex[index];
  combatParticipant.tileIndex[index] = fillTile;
  DrawOneCharacter();
  if (!CheckCombatMapCollision(dir))
  {
    --MovementRemaining;
    switch(dir)
    {
      case up:
        --combatParticipant.posY[index];
        break;
      case down:
        ++combatParticipant.posY[index];
        break;
      case left:
        --combatParticipant.posX[index];
        break;
      case right:
        ++combatParticipant.posX[index];
        break;
      default:
        break;
    }    
  }
  combatParticipant.tileIndex[index] = tempTile;
  DrawOneCharacter();
}
void PhysicalAttack(void);
void SpellAttack(void);
void ApplyCondition(void);

void DrawCombatMap(void)
{
  byte x;
  byte y;
  SetTileOrigin(viewportPosX, viewportPosY);
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
  wait_vblank(1);
  //SwapBuffer();
}

void DrawOneCharacter()
{
  if (combatParticipant.active[SelectedCharacter])
  {
    wait_vblank(1); //Reduces flicker
    DrawTileIndex = combatParticipant.tileIndex[SelectedCharacter];
    DrawTileX = combatParticipant.posX[SelectedCharacter];
    DrawTileY = combatParticipant.posY[SelectedCharacter];
    DrawTileDirect();
  }
}

void DrawCharacters(void)
{
  byte i;

  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    if (combatParticipant.active[i])
    {
      DrawTileIndex = combatParticipant.tileIndex[i];
      DrawTileX = combatParticipant.posX[i];
      DrawTileY = combatParticipant.posY[i];
      DrawTileDirect();
      wait_vblank(1);
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
    UpdateInput();
  }
  WriteLineMessageWindow("Combat End, Press Space...@", consoleDelay);
  while (!InputFire())
  {
    UpdateInput();
  }

  //Malloc free combat data
  return Map;
}