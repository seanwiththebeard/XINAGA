#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "SCREEN_ADDCHAR")
#endif

#if defined (__NES__)
#pragma code-name (push, "ADDCHAR")
#pragma rodata-name (push, "ADDCHAR")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
#pragma code-name (push, "SCREEN_ADDCHAR")
#pragma rodata-name (push, "SCREEN_ADDCHAR")
#endif

int rands = 0;

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

char namePrefixA[][8][] = 
{
        {//Human
        "Hur",
        "Dur",
        "Ran",
        "Mal",
        "Fal",
        "Lir",
        "Gaf",
        "Jex"
        },
        {//Elf
        "Lis",
        "Kes",
        "Sz'",
        "Th'",
        "Yli",
        "Lwi",
        "Eol",
        "Rys"
        },
        {//Dwarf
        "Muh",
        "Dov",
        "Sha",
        "Ste",
        "Sfo",
        "Yeg",
        "Gos",
        "Woz"
        },
        {//Construct
        "AP",
        "ZI",
        "OP",
        "UO",
        "GH",
        "QK",
        "MN",
        "ZY"
        }
};
char nameSuffixA[][8][] = 
{
        {//Human
        "kar@",
        "roth@",
        "mar@",
        "par@",
        "dak@",
        "eos@",
        "nast@",
        "stle@"
        },
        {//Elf
        "leu@",
        "ious@",
        "yir@",
        "thel@",
        "aeu@",
        "ndir@",
        "brae@",
        "owas@"
        },
        {//Dwarf
        "yen@",
        "ama@",
        "ral@",
        "apa@",
        "oru@",
        "eke@",
        "hest@",
        "julo@"
        },
        {//Construct
        "-A@",
        "-B@",
        "-C@",
        "-D@",
        "-E@",
        "-F@",
        "-G@",
        "-H@"
        }
};

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
  
  ConsoleBufferReset();
  //sprintf(strTemp, "%s %c%c%c %x@", ClassDescription[CLASS].NAME, RaceDescription[RACE].NAME[0], RaceDescription[RACE].NAME[1], RaceDescription[RACE].NAME[2], &PlayerChar[0]);
  sprintf(strTemp, "%s%s @",namePrefixA[RACE][rand()%8], nameSuffixA[RACE][rand()%8] );
  
  //ConsoleBufferAdd("Hello @");
  //ConsoleBufferAdd(strTemp);
  //ConsoleBufferAddNumber(CountRoster());
  for (i = 0; strTemp[i] != '@'; ++i)
    PlayerChar->NAME[i] = strTemp[i];
  PlayerChar->NAME[i] = '@';
        WriteLineMessageWindow(PlayerChar->NAME, 0);
        
  
  
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

  //WriteLineMessageWindow("Rolled Stats:@", 0);
  //sprintf(strTemp, "STR:%2i CON:%2i DEX:%2i WIS:%2i INT:%2i CHR:%2i@", STR, CON, DEX, WIS, INT, CHR);
  //WriteLineMessageWindow(strTemp, 0);
}

void MenuGetClassPrimeStats()
{
  byte x;
  ResetMenu("Class@", consolePosX, contextMenuPosY, 11, 10, 6);
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
        
        sprintf(strTemp, "Class Confirmed: %s @", ClassDescription[CLASS].NAME, 0);
        WriteLineMessageWindow(strTemp, 0);


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
  ResetMenu("Race@", consolePosX, contextMenuPosY, 11, 10, 5);
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
    //ConsoleBufferAdd("Race Confirmed: %s@");
    //ConsoleBufferAdd(RaceDescription[RACE].NAME);
    //ConsoleBufferPrintConsole(0);
    
    //sprintf(strTemp, "Race Confirmed: %s@ @", RaceDescription[RACE].NAME);
    //WriteLineMessageWindow(strTemp, 0);
    MenuGetClass();
  }
}

void ListParty()
{
  byte x;
  byte partyCount = CountParty();
  ResetMenu("Party@", contextMenuPosX, contextMenuPosY, contextMenuWidth, 4, partyCount);
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
  ResetMenu("Roster@", contextMenuPosX, contextMenuPosY + 7, contextMenuWidth, 8, rosterCount);
  if (rosterCount > 0)
  {

    for (x = 0; x < rosterCount; ++x)
      SetMenuItem(x, getPlayerChar(x)->NAME);
    DrawMenu();
  }
}

bool AddRandom()
{
        byte hitdice;
        RACE = rand() % 4;
        STR = RollDice(3, 6);
        CON = RollDice(3, 6);
        DEX = RollDice(3, 6);
        WIS = RollDice(3, 6);
        INT = RollDice(3, 6);
        CHR = RollDice(3, 6);
        CLASS = rand() % 6;
       
    if (RaceDescription[RACE].HITDICEMAX < ClassDescription[CLASS].HITDICE)
      HITDICE = RaceDescription[RACE].HITDICEMAX;
    else
      HITDICE = ClassDescription[CLASS].HITDICE;

    hitdice = RollDice(1, HITDICE);
    if (hitdice + AbilityModifier[CON] < 1)
    {
        sprintf(strTemp, "%s %s died, no HP@", RaceDescription[RACE].NAME, ClassDescription[CLASS].NAME);            
        WriteLineMessageWindow(strTemp, 0);
        return false;
    }
    else
    {
      HPMAX = hitdice + AbilityModifier[CON];
      HP = HPMAX;
            sprintf(strTemp, "%s %s added@", RaceDescription[RACE].NAME, ClassDescription[CLASS].NAME);
            WriteLineMessageWindow(strTemp, 0);
            
      AddToRoster();
            return true;
    }
        
}

void EditPartyMenu()
{
        ListParty();
        ListRoster();
        ResetMenu("Party@", COLS - contextMenuWidth - 1, contextMenuPosY, contextMenuWidth, 10, 6);
        SetMenuItem(0, "Next@");
        SetMenuItem(1, "Last@");
        SetMenuItem(2, "Edit@");
        SetMenuItem(3, "Remove@");
        SetMenuItem(4, "Begin@");
        SetMenuItem(5, "Back@");
        switch(GetMenuSelection())
        {
                case 0: //Next
                        {
                                break;
                        }
                case 1: //Last
                        {
                                break;
                        }
                case 2: //Edit
                        {
                                break;
                        }
                case 3: //Remove
                        {
                                break;
                        }
                case 4: //Begin Adventure
                        {
                                exitWindow = true;
                                nextScreen = MapGen;
                        }
                case 5: //Back
                        {
                                break;
                        }
        }
}

void EditRosterMenu()
{
        bool exit = false;
        byte selection = 0;
        byte rosterSelected = 0;
        //ListParty();
        ResetMenu("Roster@",COLS - contextMenuWidth - 1, contextMenuPosY, contextMenuWidth, 10, 7);
        ClearMenu();
        ListRoster();
        
        while(!exit)
        {
        SetMenuSelect(selection);
        ResetMenu("Roster@",COLS - contextMenuWidth - 1, contextMenuPosY, contextMenuWidth, 10, 7);
        SetMenuItem(0, "Next@");
        SetMenuItem(1, "Last@");
        SetMenuItem(2, "Create@");
        SetMenuItem(3, "Random@");
        SetMenuItem(4, "Delete@");
        SetMenuItem(5, "Join@");
        SetMenuItem(6, "Back@");       

        selection = GetMenuSelection();
        switch(selection)
        {
                case 0: //Next
                        {
                                ++rosterSelected;
                                SetMenuSelect(rosterSelected);                                        
                                ListRoster();
                                break;
                        }
                case 1: //Last
                        {
                                --rosterSelected;
                                SetMenuSelect(rosterSelected);                                        
                                ListRoster();
                                break;
                        }
                case 2: //Create
                        {
                                break;
                        }
                case 3: //Random
                        {
                                if(CountRoster() + CountParty() < 8)
                                {
                                        AddRandom();
                                        SetMenuSelect(rosterSelected);                                        
                                        ListRoster();
                                }
                                break;
                        }
                case 4: //Delete
                        {
                                break;
                        }
                case 5: //Join
                        {
                                break;
                        }
                case 6: //Back
                        {
                                exit = true;
                                break;
                        }
        }
        }
}

void TavernMenu()
{
        ResetMenu("Tavern@", COLS - contextMenuWidth - 1, contextMenuPosY, contextMenuWidth, 10, 4);
        ClearMenu();
        SetMenuItem(0, "Party@");
        SetMenuItem(1, "Roster@");
        SetMenuItem(2, "Save Game@");
        SetMenuItem(3, "Load Game@");
        switch(GetMenuSelection())
                {
                        case 0: //Edit Party
                                EditPartyMenu();
                                break;
                        case 1: //Edit Roster
                                EditRosterMenu();
                                break;
                        case 2: //Save Game
                                DiskSave("FILE", (byte*)&Sessions[0], sizeof(struct Session) * 4);
                                break;
                        case 3: //Load Game
                                DiskLoad("FILE", (byte*)&Sessions[0], sizeof(struct Session) * 4);
                                break;
                }
}

void MainMenu()
{
        ResetMenu("Debug Menu@",consolePosX, contextMenuPosY, 11, 10, 5);
        SetMenuItem(0, "Tavern@");
        SetMenuItem(1, "Map@");
        SetMenuItem(2, "Combat@");
        SetMenuItem(3, "Map Gen@");
        SetMenuItem(4, "Scenario@");
        switch(GetMenuSelection())
                {
                        case 0: //Tavern
                                TavernMenu();
                                break;
                        case 1: //Map
                                exitWindow = true;
                                nextScreen = Map;
                                break;
                        case 2: //Combat
                                exitWindow = true;
                                nextScreen = Combat;
                                break;
                        case 3: //Mapgen
                                exitWindow = true;
                                nextScreen = MapGen;
                                break;
                        case 4: //Scenario
                                exitWindow = true;
                                nextScreen = Scenario;
                                break;
                }
}

void MenuEditParty()
{          
        ListParty();
        ListRoster();
        ResetMenu("Party@",COLS - contextMenuWidth, contextMenuPosY, contextMenuWidth, 10, 10);
        SetMenuItem(0, "Random@");
        SetMenuItem(1, "Create@");
        SetMenuItem(2, "Delete@");
        SetMenuItem(3, "Add@");
        SetMenuItem(4, "Remove@");
        SetMenuItem(5, "Begin@");
        SetMenuItem(6, "Credits@");
        SetMenuItem(7, "Combat@");
        SetMenuItem(8, "Map Gen@");
        SetMenuItem(9, "Scenario@");    
  
  switch(GetMenuSelection())
  {
    case 0: //Random
            if(CountRoster() + CountParty() < 8)
              AddRandom();
            break;
    case 1: //Create
      {
        if (CountRoster() < 12)
          MenuGetRace();
        break;
      }
    case 2: //Delete
      {
        if (CountRoster() > 0)
          if (AreYouSure())
          {
            delete_pos(CurrentCharacter);
            return;
          }
        break;
      }
    case 3: //Add to party
      {
        if ((CountRoster() > 0) && (CountParty() < 4))
        {
          AddParty(CurrentCharacter);
          CurrentCharacter = 0;
        }
        break;
      }
    case 4: //Remove from party
      {
        if (CountParty() > 0 && CountRoster() < 12)
        {
          RemoveParty();
          CurrentCharacter = 0;
          return;
        }
        break;
      }
    case 5: //Begin Adventure
      {
        if (CountParty() > 0)
        {
          //repeatRoster = false;
          exitWindow = true;
          nextScreen = MapGen;
        }
        else
          WriteLineMessageWindow("Party Empty!@", 0);
        break;
      }
    case 6: //Debug Credits
      exitWindow = true;
      //nextScreen = Credits;
      break;
    case 7: //Combat
      exitWindow = true;
      nextScreen = Combat;
      break;
    case 8: //MapGen
      exitWindow = true;
      nextScreen = MapGen;
      break;
    case 9: //Scenario
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
        
        ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
        //WriteLineMessageWindow("Party Empty!@", 0);
        
        ScreenFadeIn();
        while (!exitWindow)
                {         
                        TavernMenu();
                        //MainMenu();
                        //MenuEditParty();
                }
        ScreenFadeOut();
        ClearScreen();
        //  ScreenFadeOut();
        return nextScreen;
}
