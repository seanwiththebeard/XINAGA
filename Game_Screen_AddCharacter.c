#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

void GetRace(void);
void DrawRoster(void);

screenName nextScreen;
byte windowX = 2;
byte windowY;
byte windowWidth = 16;
byte windowHeight = 12;
byte rosterPos;
char Selections[9][16];
int selection = 0;
byte countSelections = 5;
bool exitWindow = false;
bool nextWindow = false;
bool repeatRoster = true;
byte WindowLevel = 0;
byte CurrentCharacter = 0;
byte
    	HPMAX,
HP,
STR,
DEX,
CON,
INT,
WIS,
CHR,

RACE,
CLASS,
HITDICE;

void AddToParty()
{
  AddParty(CurrentCharacter);
  DrawRoster();
}

void RemoveFromParty()
{
  RemoveParty();
  DrawRoster();
}

void AddToRoster()
{
  struct playerChar *PlayerChar;
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
  sprintf(PlayerChar->NAME, "Hello %d @", CountRoster() - 1);

  //exitWindow = true;
  //nextWindow = true;
}

#define DrawSelection() SetChar('>', windowX + 2, windowY + selection + 1)

#define DrawCurrentCharacter() SetChar('>', windowX + 2, windowY + rosterPos + CurrentCharacter)


void SetString(char value[16], byte menuItem)
{
  byte x;
  for (x = 0; x < 16; ++x)
    Selections[menuItem][x] = value[x];

}

void MoveSelection(bool direction)
{
  SetChar(' ', windowX + 2, windowY + selection + 1);

  if (!direction && selection > 0)
    --selection;

  if (direction && selection < countSelections)
    ++selection;
  
  if (selection < 0)
    selection = 0;
  if (selection >= countSelections)
    selection = countSelections;

  DrawSelection();
}

void MoveCurrentCharacter(bool direction)
{
  SetChar(' ', windowX + 2, windowY  + rosterPos + CurrentCharacter);

  if (!direction && CurrentCharacter > 0)
    --CurrentCharacter;

  if (direction && CurrentCharacter < CountRoster() - 1)
    ++CurrentCharacter;

  if (CountRoster() > 0)
    DrawCurrentCharacter();
}

void DrawSelections()
{
  byte x;
  for (x = 0; x < countSelections + 1; ++x)
  {
    PrintString(Selections[x], windowX + 3, windowY + 1 + x, true, false);
  }
  DrawSelection();
}

void DrawCharWindow(byte xPos, byte yPos, byte width, byte height, char title[16])
{
  byte x;
  selection = 0;
  for (x = 0; x < height; ++x)
  {
    //void DrawLineH(char index, byte x, byte y, byte length);
    //DrawLineH(' ', 0, xPos, yPos + x, width);
    DrawLineH(' ', xPos, yPos + x, width);
  }

  DrawBorder(title, xPos, yPos, width, height, false);

  //PrintString(title, xPos + 1, yPos, true, false);
  DrawSelection();
  DrawSelections();
}

bool AreYouSure()
{
  windowX = 1;
  windowY = 20;
  windowWidth = 16;
  windowHeight = 5;
  countSelections = 1;
  selection = 0;
  SetString("No @", 0);
  SetString("Yes@", 1);

  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Are you sure?@");
  while (1)
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
      {
        MoveSelection(false);
      }
      if (InputDown())
      {
        MoveSelection(true);
      }
      if (InputFire())
        return selection;
    }
  }
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
  sprintf(strTemp, "STR: %d@", STR);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "CON: %d@", CON);  
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "DEX: %d@", DEX);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "WIS: %d@", WIS);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "INT: %d@", INT);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "CHR: %d@", CHR);
  WriteLineMessageWindow(strTemp, 0);
  WriteLineMessageWindow("Right to reroll@", 0);

  SetString("+ Fighter@", 0);
  SetString("+ Magic-User@", 1);
  SetString("+ Cleric@", 2);
  SetString("+ Thief@", 3);
  SetString("Exit@", 4);  

  if (STR < 9)
    Selections[0][0] = ' ';
  if (INT < 9)
    Selections[1][0] = ' ';
  if (WIS < 9)
    Selections[2][0] = ' ';
  if (DEX < 9)
    Selections[3][0] = ' ';    

  //sprintf(str, "Seed: %d@", randseed);
  //WriteLineMessageWindow(str, 0);  
}

void GetClass()
{
  int temp = 0;
  CLASS = 0;
  WindowLevel = 2;
  windowY = 10;
  windowHeight = 7;
  countSelections = 4;

  nextWindow = false;
  exitWindow = false;

  RollStats();
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Class?@");

  while (!nextWindow)
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
        MoveSelection(false);
      if (InputDown())
        MoveSelection(true);

      if (InputRight())
      {
        RollStats();
        DrawSelections();
      }
      
      if (InputFire())
      {
          if (Selections[selection][0] == ' ')
          {
            WriteLineMessageWindow("Prime stat low@", 0);
            nextWindow = false;
          }
        if (selection == countSelections)
        {
          nextWindow = true;
          exitWindow = true;
        }
        else if (Selections[selection][0] != ' ')
          nextWindow = true;
      }
    }
    if (exitWindow)
    {
      exitWindow = false;
      return;
    }
  }
  if (nextWindow)
  {
    WriteLineMessageWindow("Class Confirmed:@", 0);
    CLASS = selection;
    WriteLineMessageWindow(ClassDescription[CLASS].NAME, 0);

    if (RaceDescription[RACE].HITDICEMAX < ClassDescription[CLASS].HITDICE)
      HITDICE = RaceDescription[RACE].HITDICEMAX;
    else
      HITDICE = ClassDescription[CLASS].HITDICE;

    temp = RollDice(1, HITDICE);
    sprintf(strTemp, "Hit Dice: 1d%d@", HITDICE);
    WriteLineMessageWindow(strTemp, 0);
    sprintf(strTemp, "Roll: %d + MOD %d@", temp, AbilityModifier[CON]);
    WriteLineMessageWindow(strTemp, 0);
    if (temp + AbilityModifier[CON] < 1)
    {
      WriteLineMessageWindow("Died, no HP@", 0);
      nextWindow = false;
      //exitWindow = true;
      //nextWindow = true;
    }
    else
    {
      HPMAX = temp + AbilityModifier[CON];
      HP = HPMAX;
      AddToRoster();
    }
  }
  else
    GetClass();
}

void GetRace()
{
  RACE = 0;
  WindowLevel = 1;
  windowY = 5;
  windowHeight = 7;
  countSelections = 4;
  SetString("Human@", 0);
  SetString("Elf@", 1);
  SetString("Dwarf@", 2);
  SetString("Halfling@", 3);
  SetString("Exit@", 4);

  nextWindow = false;
  exitWindow = false;
  
  ClearScreen();
  
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Race?@");
  while (!nextWindow)
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
        MoveSelection(false);
      if (InputDown())
        MoveSelection(true);
      if (InputFire())
      {
        nextWindow = true;
        if (selection == countSelections)
        {
          exitWindow = true;
        }
      }
    }
    if (exitWindow)
    {
      exitWindow = false;
      return;
    }
  }
  if (nextWindow)
  {
    WriteLineMessageWindow("Race Confirmed:@", 0);
    WriteLineMessageWindow(Selections[selection], 0);
    RACE = selection;
    GetClass();
  }
}

void DrawRoster()
{
  byte temp = 0;
  byte partyPos;
  struct playerChar *PlayerChar = getPlayerChar(0);
  struct playerChar *PartyChar = getPartyMember(0);
  
  selection = 0;
  repeatRoster = true;
  

  sprintf(strTemp, "Count: %d@", CountRoster());
  WriteLineMessageWindow(strTemp, 0);

  countSelections = 8;
  WindowLevel = 0;
  windowX = 0;
  windowY = 0;
  windowWidth = COLS - 2;
  windowHeight = ROWS - 2;
  exitWindow = false;
  nextWindow = false;
  //CurrentCharacter = 0;

  SetString("Add selected@", 0);
  SetString("Remove last@", 1);
  SetString("Create@", 2);
  SetString("Delete@", 3);
  SetString("Start Adventure@", 4);
  SetString("Back to Title@", 5);
  SetString("Credits@", 6);
  SetString("Combat Test@", 7);
  SetString("Map Generator@", 8);

  DrawCharWindow(windowX, windowY, COLS - 2, ROWS - 2, "Edit Party@"); 
  //ListRoster
  PrintString("*Roster*@", windowX + 3, windowY + countSelections + 2, true, false);
  if (CountRoster() > 0)
  {
    rosterPos = countSelections + 4;
    for (temp = 0; temp < CountRoster(); ++temp)
    {
      sprintf(strTemp, " %s@", PlayerChar->NAME);
      PrintString(strTemp, windowX + 3, windowY + rosterPos + temp, true, false);
      sprintf(strTemp, "%s@", RaceDescription[PlayerChar->RACE].NAME);
      PrintString(strTemp, windowX + 12, windowY + rosterPos + temp, true, false);
      sprintf(strTemp, "%s@", ClassDescription[PlayerChar->CLASS].NAME);
      PrintString(strTemp, windowX + 20, windowY + rosterPos + temp, true, false);
      PlayerChar = PlayerChar->next;
    }
    DrawCurrentCharacter();
  }
  //ListParty
  
  if (CountParty() > 0)
  {
    partyPos = rosterPos + 1 + CountRoster();
    PrintString("*Party*@", windowX + 3, windowY + partyPos, true, false);
    ++partyPos;
    PartyChar = getPartyMember(0);
    for (temp = 0; temp < CountParty(); ++temp)
    {
      sprintf(strTemp, " %s@", PartyChar->NAME);
      PrintString(strTemp, windowX + 3, windowY + partyPos + temp, true, false);
      sprintf(strTemp, "%s@", RaceDescription[PartyChar->RACE].NAME);
      PrintString(strTemp, windowX + 12, windowY + partyPos + temp, true, false);
      sprintf(strTemp, "%s@", ClassDescription[PartyChar->CLASS].NAME);
      PrintString(strTemp, windowX + 20, windowY + partyPos + temp, true, false);
      PartyChar = PartyChar->next;
    }
  }
  while(repeatRoster)
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
        MoveSelection(false);
      if (InputDown()) 
        MoveSelection(true);
      if (CountRoster() > 0)
      {
        if (InputLeft())
        MoveCurrentCharacter(false);
      if (InputRight())
        MoveCurrentCharacter(true);
      }
      if (InputFire())
      {
        switch(selection)
        {
          case 0: //Add to party
            if ((CountRoster() > 0) && (CountParty() < 4))
            {
              AddToParty();
              CurrentCharacter = 0;
              MoveCurrentCharacter(false);
              return;
            }
            break;
          case 1: //Remove from party
            if (CountParty() > 0 && CountRoster() < 12)
            {
              RemoveFromParty();
              CurrentCharacter = 0;
              MoveCurrentCharacter(false);
            return;
            }
            break;
          case 2: //Create
            if (CountRoster() < 12)
            {
              GetRace();
              repeatRoster = false;
            }
            break;
          case 3: //Delete
            if (CountRoster() > 0)
              if (AreYouSure())
              {
                delete_pos(CurrentCharacter);
                return;
              }
            break;
          case 4:
            if (CountParty() > 0)
            {
              repeatRoster = false;
              exitWindow = true;
              nextScreen = Map;
            }
            else
              WriteLineMessageWindow("Party Empty!@", 0);
            break;
          case 5:
            repeatRoster = false;
            exitWindow = true;
            nextScreen = Title;
            break;
          case 6:
            repeatRoster = false;
            exitWindow = true;
            nextScreen = Credits;
            break;
          case 7:
            repeatRoster = false;
            exitWindow = true;
            nextScreen = Combat;
            break;
          case 8:
            repeatRoster = false;
            exitWindow = true;
            nextScreen = MapGen;
            break;
        }
      }
    }
  }
  //CopyDoubleBuffer();
}

screenName DrawAddCharacterScreen()
{
  exitWindow = false;
  CurrentCharacter = 0;
  srand(randseed);
  
  BlankMessageWindow(); //Why does this put characters at the very end of the screen?
  DrawMessageWindow();

  //GetRace();
  
  while (!exitWindow)
  {
    DrawRoster();
    ClearScreen();
    //CopyDoubleBuffer();
    //++CurrentCharacter;
  }
  return nextScreen;
  //CopyDoubleBufferArea(windowX, windowY, windowWidth, windowHeight);  
}