#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LC")
//#pragma rodata-name (push, "LC4")
#endif

#if defined (__NES__)
#pragma code-name (push, "COMBAT")
#pragma rodata-name (push, "COMBAT")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
#pragma code-name (push, "SCREEN_COMBAT")
#pragma rodata-name (push, "SCREEN_COMBAT")
#endif

#define fillTile 36

struct playerChar combatMonster[8];

bool ContinuingCombat;

sbyte SelectedCharacter;
sbyte SelectedTarget;
sbyte MovementRemaining;
bool CombatSuccess;
bool exitCombat;

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
bool CheckPlayersLeft(void);

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
  bool incapacitated[MaxCombatParticipants];
  bool isPlayerChar[MaxCombatParticipants];
  sbyte posX[MaxCombatParticipants];
  sbyte posY[MaxCombatParticipants];
  sbyte initiativeMod[MaxCombatParticipants];
  struct playerChar *charPointer[MaxCombatParticipants];
}combatParticipant;

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
    combatParticipant.incapacitated[i] = true;
    combatParticipant.movement[i] = 0;
  }
  SelectedCharacter = 0;
}

void Initialize(void)
{
  exitCombat = false;
  ContinuingCombat = false;
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
    combatParticipant.incapacitated[i] = false;
    combatParticipant.movement[i] = getPartyMember(i)->DEX / 2;
    combatParticipant.charPointer[i] = getPartyMember(i);
    ++SelectedCharacter;
  }
}

void GetMonsters(void)
{
  byte i;
  byte LastMonster = SelectedCharacter + MonsterCount;
  byte monstersSet = 0;
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
    combatParticipant.incapacitated[i] = false;
    combatParticipant.movement[i] = 4;
    combatParticipant.targetIndex[i] = rand() % CountParty();

    combatParticipant.charPointer[i] = &combatMonster[monstersSet];

    strcpy(combatMonster[monstersSet].NAME,  "Monster");
    combatMonster[monstersSet].HP = 1;
    combatMonster[monstersSet].HPMAX = 1;
    combatMonster[monstersSet].STR = 10;
    combatMonster[monstersSet].INT = 10;
    combatMonster[monstersSet].WIS = 10;
    combatMonster[monstersSet].DEX = 10;
    combatMonster[monstersSet].CHR = 10;
    combatMonster[monstersSet].CON = 10;
    combatMonster[monstersSet].WEAPON = 1;
    combatMonster[monstersSet].ARMOR = 1;    
    
    ++monstersSet;
    
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
    WriteLineMessageWindow(strTemp, 0);
  }
  else
  {
    if (combatParticipant.isPlayerChar[SelectedCharacter])
      WriteLineMessageWindow("Moved, press space@", 0);
    else
      WriteLineMessageWindow("Monster moved@", 0);
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
  WriteLineMessageWindow("@",0);
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
      if (!combatParticipant.incapacitated[SelectedCharacter])
        found = true;

    ++count;
    if (count > MaxCombatParticipants)
    {
      WriteLineMessageWindow("No Entities@", consoleDelay);
      exitCombat = true;
      return false;
    }
  }
  return true;
}

void DoCombatRound()
{
  if(!CheckPlayersLeft())
    {
      exitCombat = true;
      WriteLineMessageWindow("Defeated...@", consoleDelay);
      nextScreen = EditParty;
      return;
    }
  
  RollInitiative();

  if (SelectNextCharacter())
    GetActionSelection();
  //else
  {
    //exitCombat = true;
  }
}

void PhysicalAttack()
{
  byte targetAC = combatParticipant.charPointer[SelectedTarget]->DEX;
  byte rollToHit = rand() % 20;
  byte damage = 5;
  int targetHP = combatParticipant.charPointer[SelectedTarget]->HP;

  if (SelectedTarget < 0)
  {
    WriteLineMessageWindow("No target!@", 0);    
    return;
  }
  
  ConsoleBufferReset();
  
  if (rollToHit >= targetAC)
  {
    targetHP -= damage;

    sprintf(strTemp, "%s hits %s for %d damage@", combatParticipant.charPointer[SelectedCharacter]->NAME, combatParticipant.charPointer[SelectedTarget]->NAME, damage);
    WriteLineMessageWindow(strTemp, 0);

    if(targetHP <= 0)
    {
      combatParticipant.active[SelectedTarget] = false;
      targetHP = 0;
      sprintf(strTemp, "%s fell@", combatParticipant.charPointer[SelectedTarget]->NAME);
      WriteLineMessageWindow(strTemp, 0);
      combatParticipant.incapacitated[SelectedTarget] = true;
    }
    combatParticipant.charPointer[SelectedTarget]->HP = targetHP;
    
    if(combatParticipant.isPlayerChar[SelectedTarget])
      DrawCharStats();
    
    DrawCombatMap();
  }
  else
  {
    sprintf(strTemp, "%s missed %s@", combatParticipant.charPointer[SelectedCharacter]->NAME, combatParticipant.charPointer[SelectedTarget]->NAME);
    WriteLineMessageWindow(strTemp, 0);
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
  WriteLineMessageWindow("Select Target@",0);

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
            if(combatParticipant.active[i])
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
      //sprintf(strTemp, "Wander Failed M#%d@", SelectedCharacter);
     // WriteLineMessageWindow(strTemp, 0);
    }
  }
  ClearArrow();
  WriteRemainingMovement();
}

bool CheckEnemiesLeft()
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
        if ((combatParticipant.isPlayerChar[i] == false) && (combatParticipant.active[i] == true))
          return true;

  return false;
}


bool CheckPlayersLeft()
{
  byte i;
  for (i = 0; i < MaxCombatParticipants; ++i)
        if ((combatParticipant.isPlayerChar[i] == true) && (combatParticipant.active[i] == true))
          return true;

  return false;
}
//Actions
void SelectMonsterAction(void)
{
  //SelectPlayerAction();
  //sprintf(strTemp, "Monster Action %d@", SelectedCharacter);
  //WriteLineMessageWindow(strTemp, consoleDelay);
  MonsterWander();
}

void Flee()
{
  byte fleeRoll = rand() % 20;
  sprintf(strTemp, "Flee attempt rolled %d against 12@", fleeRoll);
      WriteLineMessageWindow(strTemp, 0);
  if (fleeRoll > 12)
  {
      exitCombat = true;
      WriteLineMessageWindow("Fled!@", 0);
  }
  else
      WriteLineMessageWindow("Couldn't escape...@", 0);
}

void SelectPlayerAction(void)
{
  byte tempTile;
  bool finished = false;
  byte moveX = combatParticipant.posX[SelectedCharacter];
  byte moveY = combatParticipant.posY[SelectedCharacter];

  if(!ContinuingCombat)
  {
    if (!CheckEnemiesLeft())
    {
      WriteLineMessageWindow("No targets, continue?@",0);
      if (!AreYouSure())
      {
        exitCombat = true;
        nextScreen = Map;
        return;
      }
      else
        ContinuingCombat = true;
    }
  }

  ResetMenu("Action@", contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 6, true);
  SetMenuItem(0, "Move@");
  SetMenuItem(1, "Attack@");
  SetMenuItem(2, "Magic@");
  SetMenuItem(3, "Item@");
  SetMenuItem(4, "Flee@");
  SetMenuItem(5, "End Turn@");

  while (!finished)
  {
    WriteLineMessageWindow("Command?@",0);
    if(combatParticipant.active[SelectedCharacter])
      DrawArrow(combatParticipant.posX[SelectedCharacter], combatParticipant.posY[SelectedCharacter]);
    switch (GetMenuSelection())
    {
      case 0: // Move
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
      case 1: // Attack
        if (combatParticipant.active[SelectedCharacter])
        {
          GetTargetSelection();
          PhysicalAttack();
          finished = true;
        }
        break;
      case 2: // Magic
        if (combatParticipant.active[SelectedCharacter])
        {
          finished = true;
        }
        break;
      case 3: // Item
        if (combatParticipant.active[SelectedCharacter])
        {
          finished = true;
        }
        break;
      case 4: // Flee
        if(ContinuingCombat)
        {
          exitCombat = true;
        }
        else
        {
          Flee();
        }
          finished = true;
        break;
      case 5: // End Turn
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
        if (!combatParticipant.incapacitated[i])
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
      //DrawTileIndex = fillTile;
      //DrawTileX = x;
      //DrawTileY = y;
      tilePosX = x;
      tilePosY = y;
      DrawTileSeq(fillTile);
    }
  DrawCharacters();
  //wait_vblank(1);
  //SwapBuffer();
}

void DrawOneCharacter()
{
  if (combatParticipant.active[SelectedCharacter])
  {
    //wait_vblank(1); //Reduces flicker
    //DrawTileIndex = combatParticipant.tileIndex[SelectedCharacter];
    //DrawTileX = combatParticipant.posX[SelectedCharacter];
    //DrawTileY = combatParticipant.posY[SelectedCharacter];
    //DrawTileDirect();
    tilePosX = combatParticipant.posX[SelectedCharacter];
    tilePosY = combatParticipant.posY[SelectedCharacter];
    DrawTileSeq(combatParticipant.tileIndex[SelectedCharacter]);
  }
}

void DrawCharacters(void)
{
  byte i;

  for (i = 0; i < MaxCombatParticipants; ++i)
  {
    if (combatParticipant.active[i])
    {
      //DrawTileIndex = combatParticipant.tileIndex[i];
      //DrawTileX = combatParticipant.posX[i];
      //DrawTileY = combatParticipant.posY[i];
      //DrawTileDirect();
      tilePosX = combatParticipant.posX[i];
      tilePosY = combatParticipant.posY[i];
      DrawTileSeq(combatParticipant.tileIndex[i]);
      //wait_vblank(1);
    }
  }
}

screenName Update_Combat(void)
{
  Initialize();
  //randseed = 0;
  //srand(randseed);
  ScreenFadeIn();

  while (!exitCombat)
  {
    ConsoleBufferReset();
    DoCombatRound();
    UpdateInput();
  }
  WaitForInput();
  while (!InputFire())
  {
    UpdateInput();
  }

  //Malloc free combat data
  ScreenFadeOut();
  return nextScreen;
}
