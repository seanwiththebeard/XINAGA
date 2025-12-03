#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "ADDCHAR")
#pragma rodata-name (push, "ADDCHAR")
#pragma data-name (push, "XRAM")
#pragma bss-name (push, "XRAM")
#endif

screenName nextScreen;

bool AreYouSure();
bool exitWindow;
byte CurrentCharacter;
byte HPMAX;
byte HP;
byte STR;
byte DEX;
byte CON;
byte INT;
byte WIS;
byte CHR;
byte RACE;
byte CLASS;
byte HITDICE;

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
  sprintf(strTemp, "NAME%X@", &PlayerChar[0]);
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
  sprintf(strTemp, "STR:%2i CON:%2i DEX:%2i WIS:%2i INT:%2i CHR:%2i@", STR, CON, DEX, WIS, INT, CHR);
  WriteLineMessageWindow(strTemp, 0);
  //sprintf(strTemp, "(WIS: %2d) (INT: %2d) (CHR: %2d)@", WIS, INT, CHR);
  //WriteLineMessageWindow(strTemp, 0);
}

void MenuGetClassPrimeStats()
{
  byte x;
  ResetMenu("Class@",22, 2, 8, 9, 6);
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
        return;
    }
  }
    WriteLineMessageWindow("Class Confirmed:@", 0);
    WriteLineMessageWindow(ClassDescription[CLASS].NAME, 0);

    if (RaceDescription[RACE].HITDICEMAX < ClassDescription[CLASS].HITDICE)
      HITDICE = RaceDescription[RACE].HITDICEMAX;
    else
      HITDICE = ClassDescription[CLASS].HITDICE;

    hitdice = RollDice(1, HITDICE);
    sprintf(strTemp, "Hit Dice: 1d%i@", HITDICE);
    WriteLineMessageWindow(strTemp, 0);
    sprintf(strTemp, "Roll: %i + MOD %i@", hitdice, AbilityModifier[CON]);
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
  ResetMenu("Race@", 12, 2, 9, 9, 5);
  SetMenuItem(0, RaceDescription[0].NAME);
  SetMenuItem(1, RaceDescription[1].NAME);
  SetMenuItem(2, RaceDescription[2].NAME);
  SetMenuItem(3, RaceDescription[3].NAME);
  SetMenuItem(4, "Exit@");
  RACE = GetMenuSelection();
  if (RACE == 4)
    ClearMenu();
  else
  {
    ConsoleBufferReset();
    ConsoleBufferAdd("Race Confirmed:@");
    ConsoleBufferAdd(RaceDescription[RACE].NAME);
    ConsoleBufferPrintConsole(0);
    MenuGetClass();
  }
}

void ListParty()
{
  byte x;
  byte partyCount = CountParty();
  ResetMenu("Party@", contextMenuPosX, contextMenuPosY, contextMenuWidth, partyCount + 1, partyCount);
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
  ResetMenu("Roster@", contextMenuPosX, contextMenuPosY + CountParty() + 2, contextMenuWidth, rosterCount + 1, rosterCount);
  if (rosterCount > 0)
  {

    for (x = 0; x < rosterCount; ++x)
      SetMenuItem(x, getPlayerChar(x)->NAME);
    DrawMenu();
  }
}

void MenuEditParty()
{
  ListParty();
  ListRoster();
  
  ResetMenu("Party@",consolePosX, contextMenuPosY, 9, 9, 9);
  SetMenuItem(0, "Create@");
  SetMenuItem(1, "Delete@");
  SetMenuItem(2, "Add@");
  SetMenuItem(3, "Remove@");
  SetMenuItem(4, "Begin@");
  SetMenuItem(5, "Credits@");
  SetMenuItem(6, "Combat@");
  SetMenuItem(7, "Map Gen@");
  SetMenuItem(8, "Scenario@");
  
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  ScreenFadeIn();
  
  //sprintf(strTemp, "Address %4x %s@", &RaceDescription[0], &RaceDescription[0]);
    //WriteLineMessageWindow(strTemp, 0);
  
  
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
  //ClearMenu();
  //ClearScreen();
}

screenName DrawAddCharacterScreen()
{
  ScreenFadeOut();
  nextScreen = EditParty;
  exitWindow = false;
  CurrentCharacter = 0;
  srand(randseed);
  ClearScreen();
  
  while (!exitWindow)
  {  
    MenuEditParty();
    ScreenFadeOut();
    ClearScreen();
  }
  ScreenFadeOut();
  return nextScreen;
}
