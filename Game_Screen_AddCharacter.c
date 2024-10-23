#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "STARTUP")
//#pragma data-name (push, "XRAM")
#pragma bss-name (push, "XRAM")

#endif

screenName nextScreen = EditParty;
#define consolePosX  2
#define consolePosY ROWS - 1 - consoleHeight - 2
#define consoleWidth COLS - 10
#define consoleHeight 6
bool AreYouSure();
bool exitWindow = false;
byte CurrentCharacter = 0;
byte HPMAX = 0;
byte HP = 0;
byte STR = 0;
byte DEX = 0;
byte CON = 0;
byte INT = 0;
byte WIS = 0;
byte CHR = 0;
byte RACE = 0;
byte CLASS = 0;
byte HITDICE = 0;

void AddToRoster()
{
  struct playerChar *PlayerChar;
  byte i;
  create();
  PlayerChar = getPlayerChar(CountRoster() - 1);
  PlayerChar->HPMAX = HPMAX;
  PlayerChar->HP = HP;
  PlayerChar->STR = STR;
  PlayerChar->DEX = DEX;
  PlayerChar->CON = CON;
  PlayerChar->INT = INT;
  PlayerChar->WIS = WIS;
  PlayerChar->CHR = CHR;
  PlayerChar->RACE = RACE;
  PlayerChar->CLASS = CLASS;
  PlayerChar->inParty = false;
  
  
  ConsoleBufferReset();
  sprintf(strTemp, "Hello %X@", &PlayerChar[0]);
  //ConsoleBufferAdd("Hello @");
  //ConsoleBufferAdd(strTemp);
  //ConsoleBufferAddNumber(CountRoster());
  for (i = 0; strTemp[i] != '@'; ++i)
    PlayerChar->NAME[i] = strTemp[i];
  PlayerChar->NAME[i] = '@';
  
  
  //ConsoleBufferReset();
  //sprintf(strTemp, "Address: %X@", &PlayerChar[0]);
  //ConsoleBufferAdd(strTemp);
  //ConsoleBufferPrintConsole(0);
  //AreYouSure();
  //sprintf(PlayerChar->NAME, strTemp);
}

//#define DrawSelection() SetChar('>', windowX + 2, windowY + selection + 1)

//#define DrawCurrentCharacter() SetChar('>', windowX + 2, windowY + rosterPos + CurrentCharacter)

bool AreYouSure()
{
  ResetMenu("Sure?@",consolePosX + consoleWidth + 1, 20, 5, 2, 2);
  SetMenuItem(0, "No @");
  SetMenuItem(1, "Yes@");
  
  return (GetMenuSelection());
}

void RollStats()
{
  STR = RollDice(3, 6);
  CON = RollDice(3, 6);
  DEX = RollDice(3, 6);
  WIS = RollDice(3, 6);
  INT = RollDice(3, 6);
  CHR = RollDice(3, 6);

  WriteLineMessageWindow("Rolled Stats:@", 0);
  sprintf(strTemp, "(STR: %2d) (CON: %2d) (DEX %2d)@", STR, CON, DEX);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "(WIS: %2d) (INT: %2d) (CHR %2d)@", WIS, INT, CHR);
  WriteLineMessageWindow(strTemp, 0);
}

void MenuGetClassPrimeStats()
{
  byte x;
  ResetMenu("Class@",22, 2, 11, 9, 6);
  for (x = 0; x < 4; ++x)
  {
    SetMenuItem(x, ClassDescription[x].NAME);
  }
  
  if (STR >= 9)
    HighlightMenuItem(0);
  if (INT >= 9)
    HighlightMenuItem(1);
  if (WIS >= 9)
    HighlightMenuItem(2);
  if (DEX >= 9)
    HighlightMenuItem(3);
  
  SetMenuItem(4, "Reroll@");
  SetMenuItem(5, "Exit@");
}
void WaitForInput(void)
{
  bool exit = false;
  SetLineMessageWindow("Press space to continue@", 0);
  
  while (!exit)
  {
    UpdateInput();
    if (InputFire())
      exit = true;
  }
}
void MenuGetClass()
{
  byte hitdice;
  RollStats();
  MenuGetClassPrimeStats();  
  CLASS = GetMenuSelection();
  while(!IsMenuItemHighlighted(CLASS))
  {
    if (CLASS < 4)
    {
      WriteLineMessageWindow("Prime stat low@", 0);
      CLASS = GetMenuSelection();
    }
    else
    {
      if (CLASS == 4) //Reroll
      {
        RollStats();
        MenuGetClassPrimeStats();
        CLASS = GetMenuSelection();
      }
      if (CLASS == 5)
      {
        ClearScreen();
        return;
      }
    }
    
  }
    WriteLineMessageWindow("Class Confirmed:@", 0);
    WriteLineMessageWindow(ClassDescription[CLASS].NAME, 0);

    if (RaceDescription[RACE].HITDICEMAX < ClassDescription[CLASS].HITDICE)
      HITDICE = RaceDescription[RACE].HITDICEMAX;
    else
      HITDICE = ClassDescription[CLASS].HITDICE;

    hitdice = RollDice(1, HITDICE);
    sprintf(strTemp, "Hit Dice: 1d%d@", HITDICE);
    WriteLineMessageWindow(strTemp, 0);
    sprintf(strTemp, "Roll: %d + MOD %d@", hitdice, AbilityModifier[CON]);
    WriteLineMessageWindow(strTemp, 0);
    if (hitdice + AbilityModifier[CON] < 1)
    {
      WriteLineMessageWindow("Died, no HP@", 0);
    }
    else
    {
      HPMAX = hitdice + AbilityModifier[CON];
      HP = HPMAX;
      AddToRoster();
    }
      WaitForInput();
}

void MenuGetRace()
{
  ResetMenu("Race@", 13, 2, 8, 9, 5);
  SetMenuItem(0, RaceDescription[0].NAME);
  SetMenuItem(1, RaceDescription[1].NAME);
  SetMenuItem(2, RaceDescription[2].NAME);
  SetMenuItem(3, RaceDescription[3].NAME);
  SetMenuItem(4, "Exit@");
  RACE = GetMenuSelection();
  if (RACE != 4)
  {
    ConsoleBufferReset();
    ConsoleBufferAdd("Race Confirmed:@");
    ConsoleBufferAdd(RaceDescription[RACE].NAME);
    ConsoleBufferPrintConsole(0);
    MenuGetClass();
  }
  ClearScreen();
}

void ListParty()
{
  byte x;
  byte partyCount = CountParty();
  ResetMenu("Party Members@", 17, 12, 13, 4, partyCount);
  if (partyCount > 0)
  {

    for (x = 0; x < partyCount; ++x)
    {
      SetMenuItem(x, getPartyMember(x)->NAME);
    }
    DrawMenu();
  }
}

void ListRoster()
{
  byte x;
  byte rosterCount = CountRoster();
  ResetMenu("Roster Members@",2, 12, 14, 4, rosterCount);
  if (rosterCount > 0)
  {

    for (x = 0; x < rosterCount; ++x)
      SetMenuItem(x, getPlayerChar(x)->NAME);
    DrawMenu();
  }
}

void MenuEditParty()
{
  ListRoster();
  ListParty();
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  
  ResetMenu("Edit Party@",2, 2, 10, 9, 9);
  SetMenuItem(0, "Create@");
  SetMenuItem(1, "Delete@");
  SetMenuItem(2, "Add@");
  SetMenuItem(3, "Remove@");
  SetMenuItem(4, "Begin@");
  SetMenuItem(5, "Credits@");
  SetMenuItem(6, "Combat@");
  SetMenuItem(7, "Map Gen@");
  SetMenuItem(8, "Scenario@");
  
  switch(GetMenuSelection())
  {
    case 0: //Create
      {
        if (CountRoster() < 12)
          MenuGetRace();
        break;
      }
    case 1: //Delete
      {
        if (CountRoster() > 0)
          if (AreYouSure())
          {
            delete_pos(CurrentCharacter);
            return;
          }
        break;
      }
    case 2: //Add to party
      {
        if ((CountRoster() > 0) && (CountParty() < 4))
        {
          AddParty(CurrentCharacter);
          CurrentCharacter = 0;
        }
        break;
      }
    case 3: //Remove from party
      {
        if (CountParty() > 0 && CountRoster() < 12)
        {
          RemoveParty();
          CurrentCharacter = 0;
          return;
        }
        break;
      }
    case 4: //Begin Adventure
      {
        if (CountParty() > 0)
        {
          //repeatRoster = false;
          exitWindow = true;
          nextScreen = Map;
        }
        else
          WriteLineMessageWindow("Party Empty!@", 0);
        break;
      }
    case 5: //Debug Credits
      exitWindow = true;
      //nextScreen = Credits;
      break;
    case 6:
      exitWindow = true;
      nextScreen = Combat;
      break;
    case 7:
      exitWindow = true;
      nextScreen = MapGen;
      break;
    case 8:
      exitWindow = true;
      nextScreen = Scenario;
      break;
  }  
}

screenName DrawAddCharacterScreen()
{
  exitWindow = false;
  CurrentCharacter = 0;
  srand(randseed);

  while (!exitWindow)
  {
    ClearScreen();
    MenuEditParty();
  }
  ClearScreen();
  return nextScreen;
}