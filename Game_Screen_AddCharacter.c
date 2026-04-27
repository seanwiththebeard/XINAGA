#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LC")
//#pragma rodata-name (push, "LC")
#endif

#if defined (__NES__)
#pragma code-name (push, "ADDCHAR")
#pragma rodata-name (push, "ADDCHAR")
//#pragma data-name (push, "XRAM")
#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
#pragma code-name (push, "SCREEN_ADDCHAR")
#pragma rodata-name (push, "SCREEN_ADDCHAR")
#endif

int rands;
screenName nextScreen;
bool AreYouSure();
bool exitWindow;
sbyte CurrentCharacter;
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

void DrawStatsSelected();

/*
{"Human", 255},
  {"Elf", 6},
  {"Dwarf", 255},
  {"Halfling", 6},
  {"Orc", 6},
  {"Fuzzer", 6},
  {"Dracon", 6},
  {"Construct", 6}
*/

void DrawPartyStats()
{
        DrawBorder("Stats@", viewportPosX - 1 , viewportPosY + 4, viewportWidth * 2 + 2, 5, true);
        ConsoleBufferReset();
        sprintf(strTemp, "GOLD:        %05u@", Sessions[0].GOLD);
        ConsoleBufferPrint(viewportPosX, viewportPosY + 5);
        sprintf(strTemp, "STEPS:       %05u@", Sessions[0].STEPS);
        ConsoleBufferPrint(viewportPosX, viewportPosY + 6);
        sprintf(strTemp, "YEAR%02i MON%02i DAY%02i@", Sessions[0].SOLAR, Sessions[0].LUNAR, Sessions[0].MINAR);
        ConsoleBufferPrint(viewportPosX, viewportPosY + 7);
}

void AddToRoster()
{
  struct playerChar *PlayerChar;
  byte i;
       ConsoleBufferReset();
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


        sprintf(strTemp, "%s %s %s@", RaceDescription[RACE].NAME, ClassDescription[CLASS].NAME, PlayerChar->NAME);
            WriteLineMessageWindow(strTemp, 0);
        //WriteLineMessageWindow(PlayerChar->NAME, 0);
  //ConsoleBufferReset();
  //sprintf(strTemp, "Address: %X@", &PlayerChar[0]);
  //ConsoleBufferAdd(strTemp);
  //ConsoleBufferPrintConsole(0);
  //AreYouSure();
  //sprintf(PlayerChar->NAME, strTemp);
  DrawStatsSelected();
}

//#define DrawSelection() SetChar('>', windowX + 2, windowY + selection + 1)

//#define DrawCurrentCharacter() SetChar('>', windowX + 2, windowY + rosterPos + CurrentCharacter)

bool AreYouSure()
{
  ResetMenu("Sure?@", contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 2, true);
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
  WriteLineMessageWindow("@", 0);
  sprintf(strTemp, "STR:%2i CON:%2i DEX:%2i WIS:%2i INT:%2i CHR:%2i@", STR, CON, DEX, WIS, INT, CHR);
  WriteLineMessageWindow(strTemp, 0);
}

void MenuGetClassPrimeStats()
{
  byte x;
  ResetMenu("Class@", contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 10, true);
  for (x = 0; x < 8; ++x)
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
  SetMenuItem(8, "Reroll@");
  SetMenuItem(9, "Exit@");
}

void MenuGetClass()
{
  byte hitdice;
  RollStats();
  MenuGetClassPrimeStats();
  CLASS = GetMenuSelection();

  while(!IsMenuItemHighlighted(CLASS))
  {
    if (CLASS < 8)
    {
      WriteLineMessageWindow("Prime stat low@", 0);
      CLASS = GetMenuSelection();
    }
    else
    {
      if (CLASS == 8) //Reroll
      {
        RollStats();
        MenuGetClassPrimeStats();
        CLASS = GetMenuSelection();
      }
      if (CLASS == 9)
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
  ResetMenu("Race@", contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 9, true);
  SetMenuItem(0, RaceDescription[0].NAME);
  SetMenuItem(1, RaceDescription[1].NAME);
  SetMenuItem(2, RaceDescription[2].NAME);
  SetMenuItem(3, RaceDescription[3].NAME);
  SetMenuItem(4, RaceDescription[4].NAME);
  SetMenuItem(5, RaceDescription[5].NAME);
  SetMenuItem(6, RaceDescription[6].NAME);
  SetMenuItem(7, RaceDescription[7].NAME);
  SetMenuItem(8, "Exit@");
  RACE = GetMenuSelection();
  if (RACE == 8)
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
        if(CurrentCharacter < 0)
        CurrentCharacter = 0;
  ResetMenu("Party@", 1, 1, viewportWidth * 2 - 1, 4, partyCount, true);
  if (partyCount > 0)
  {
    for (x = 0; x < partyCount; ++x)
    {
      SetMenuItem(x, getPartyMember(x)->NAME);
    }
  }
        //DrawMenu();
}

/*
typedef struct
  char *NAME;
  byte HITDICEMAX;
  byte ATTRIB_BONUS;
  byte ATTRIB_PENALTY;
  byte TRAITS;
  byte ELEMENT_WEAKNESS;
  byte ELEMENT_RESIST;
  byte CONDITION_RESIST;
}RaceDescriptionDef;

  char *NAME;
  byte HITDICE;
  byte ATTRIB_DEP;
  byte WEAPON_PROF;
  byte ARMOR_PROF;
  byte SPELLS_WIZ;
  byte SPELLS_CLER;
  byte SONGS_BARD;
}ClassDescriptionDef;
*/

void DrawStatsSelected()
{
  //byte class = getPlayerChar(CurrentCharacter)->CLASS;
  //byte race = getPlayerChar(CurrentCharacter)->RACE;

  //Resize the console and just print all of this there
  
  ResetMenu("Selected@", 1, 10, viewportWidth * 2, viewportHeight, viewportHeight, true);
  sprintf(strTemp, "%s@", RaceDescription[RACE].NAME);
  ConsoleBufferPrint(1, 10);
  sprintf(strTemp, "%s@ ", ClassDescription[CLASS].NAME);
  ConsoleBufferPrint(2, 11);
  sprintf(strTemp, "Traits %d@ ", RaceDescription[RACE].TRAITS);
  ConsoleBufferPrint(2, 12);
  sprintf(strTemp, "Element Weak %d@ ", RaceDescription[RACE].ELEMENT_WEAKNESS);
  ConsoleBufferPrint(2, 13);
  sprintf(strTemp, "Element Resist %d@ ", RaceDescription[RACE].ELEMENT_RESIST);
  ConsoleBufferPrint(2, 14);
  sprintf(strTemp, "Condition Resist %d@ ", RaceDescription[RACE].CONDITION_RESIST);
  ConsoleBufferPrint(2, 15);
  sprintf(strTemp, "Weapon Prof %d@ ", ClassDescription[CLASS].WEAPON_PROF);
  ConsoleBufferPrint(2, 16);
  sprintf(strTemp, "Armor Prof %d@ ", ClassDescription[CLASS].ARMOR_PROF);
  ConsoleBufferPrint(2, 17);
  
  
  
  
}

void ListRoster()
{
        byte x;
        byte rosterCount = CountRoster();

        if(CurrentCharacter < 0)
        CurrentCharacter = 0;
        ResetMenu("Roster@", 1, 10, viewportWidth * 2, viewportHeight, rosterCount, true);
        if (rosterCount > 0)
        {
                for (x = 0; x < rosterCount; ++x)
                SetMenuItem(x, getPlayerChar(x)->NAME);
        }
        //DrawMenu();
}

bool AddRandom(byte class, byte race)
{
        byte hitdice;
        RACE = race;
        CLASS = class;
        RollStats();
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
      AddToRoster();
            return true;
    }
}

void MenuParty(bool clear)
{
        ResetMenu("Party@", contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 6, clear);
                SetMenuItem(0, "Next@");
                SetMenuItem(1, "Last@");
                SetMenuItem(2, "Edit@");
                SetMenuItem(3, "Remove@");
                SetMenuItem(4, "Begin@");
                SetMenuItem(5, "Back@");
}

void EditPartyMenu()
{
        bool exit = false;
        byte selection = 0;
        ClearMenu();
        CurrentCharacter = 0;
        ListParty();
        //ListRoster();
        SetMenuSelect(selection);
        MenuParty(true);

        while(!exit)
        {
                SetMenuSelect(selection);
                MenuParty(false);
                selection = GetMenuSelection();
                switch(selection)
                {
                        case 0: //Next
                                {
                                        if (CurrentCharacter < (CountParty()-1))
                                        {
                                                ++CurrentCharacter;
                                                SetMenuSelect(CurrentCharacter);
                                                ListParty();
                                        }
                                        break;
                                }
                        case 1: //Last
                                {
                                        if(CurrentCharacter > 0)
                                        {
                                                --CurrentCharacter;
                                                SetMenuSelect(CurrentCharacter);
                                                ListParty();
                                        }
                                        break;
                                }
                        case 2: //Edit
                                {
                                        break;
                                }
                        case 3: //Remove
                                {
                                        if (CountParty() > 0 && CountRoster() < 12)
                                        {
                                                RemoveParty(CurrentCharacter);
                                                --CurrentCharacter;
                                                ListRoster();
                                                SetMenuSelect(CurrentCharacter);
                                                ListParty();
                                        }
                                        break;
                                }
                        case 4: //Begin Adventure
                                {
                                        exitWindow = true;
                                        nextScreen = MapGen;
                                }
                        case 5: //Back
                                {
                                        exit = true;
                                        break;
                                }
                }
        }
}


void MenuRoster(bool clear)
{
        ResetMenu("Roster@",contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 7, clear);
        SetMenuItem(0, "Next@");
        SetMenuItem(1, "Last@");
        SetMenuItem(2, "Create@");
        SetMenuItem(3, "Random@");
        SetMenuItem(4, "Delete@");
        SetMenuItem(5, "Join@");
        SetMenuItem(6, "Back@");
}
void EditRosterMenu()
{
        bool exit = false;
        byte selection = 0;
        //ClearMenu();
        CurrentCharacter = 0;
        //ListParty();
        ListRoster();
        MenuRoster(true);
        while(!exit)
        {
        SetMenuSelect(selection);
        MenuRoster(false);
        selection = GetMenuSelection();
        switch(selection)
        {
                case 0: //Next
                        {
                                if (CurrentCharacter < (CountRoster()-1))
                                {
                                        ++CurrentCharacter;
                                        SetMenuSelect(CurrentCharacter);
                                        //ListRoster();
                                }
                                break;
                        }
                case 1: //Last
                        {
                                if(CurrentCharacter > 0)
                                {
                                        --CurrentCharacter;
                                        SetMenuSelect(CurrentCharacter);
                                        //ListRoster();
                                }
                                break;
                        }
                case 2: //Create
                        {
                                if (CountRoster() < 12)
                                {
                                        MenuGetRace();
                                        ListParty();
                                        //ListRoster();
                                }
                                break;
                        }
                case 3: //Random
                        {
                                if(CountRoster() + CountParty() < 8)
                                {
                                        AddRandom(rand() % 8, rand() % 8);
                                        SetMenuSelect(CurrentCharacter);
                                        ListParty();
                                        //ListRoster();
                                }
                                break;
                        }
                case 4: //Delete
                        {
                                if (CountRoster() > 0)
                                        if (AreYouSure())
                                        {
                                                delete_pos(CurrentCharacter);
                                                ListRoster();
                                                return;
                                        }
                                break;
                        }
                case 5: //Join
                        {
                                if ((CountRoster()) && (CountParty() < 4))
                                {
                                        AddParty(CurrentCharacter);
                                        --CurrentCharacter;
                                        ListParty();
                                        SetMenuSelect(CurrentCharacter);
                                }
                                break;
                        }
                case 6: //Back
                        {
                                exit = true;
                                break;
                        }
        }
                                        ListRoster();
        }
}

void TavernMenu()
{
        ResetMenu("Tavern@", contextMenuPosX, contextMenuPosY, contextMenuWidth, contextMenuHeight, 4, true);
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
                                DiskSave("file", (byte*)&Sessions[0], sizeof(struct Session) * 4);
                                break;
                        case 3: //Load Game
                                DiskLoad("file", (byte*)&Sessions[0], sizeof(struct Session) * 4);
                                break;
                }
}

const byte predefcharindex[] = {2, 7, 5, 3};
screenName DrawAddCharacterScreen()
{
        nextScreen = EditParty;
        exitWindow = false;
        CurrentCharacter = 0;
        srand(477);
        ScreenFadeIn();
        ListParty();
        DrawPartyStats();
        ListRoster();
        DrawCharStats();
  if (CountParty() == 0)
  {
    while(CountParty() < 4)
    {
      while(!AddRandom(predefcharindex[CountParty()], rand() % 8));
      AddParty(CurrentCharacter);
      ListParty();
      DrawPartyStats();
      ListRoster();
      DrawCharStats();
    }
    return MapGen;
  }
        //while(CountParty() != 4)
        //
                //ListParty();
                //DrawPartyStats();
                //ListRoster();
                ////AddRandom(rand() % 8, CountRoster() + 2);
                //if (CountRoster())
                //{
                        //AddParty(CurrentCharacter);
                        //ListParty();
                       // DrawCharStats();
                //}
      //  }
        //return MapGen;
        while (!exitWindow)
                {
                        TavernMenu();
                }
        return nextScreen;
}

const char namePrefixA[8][8][8] =
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
        {//Halfling
        "Flo",
        "Bli",
        "Man",
        "Tem",
        "Huw",
        "Jle",
        "Nel",
        "Sul"
        },
        {//Orc
        "Mor",
        "Far",
        "Hir",
        "Gha",
        "Mal",
        "Ral",
        "Dan",
        "Pol"
        },
        {//Fuzzer
        "Uwe",
        "Reu",
        "Bes",
        "Kle",
        "Uye",
        "Xes",
        "Ztu",
        "Hsa"
        },
        {//Dracon
        "Fah",
        "Sza",
        "Oha",
        "Wah",
        "Lah",
        "Rah",
        "Qua",
        "Pla"
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
const char nameSuffixA[8][8][8] =
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
        {//Halfling
        "bo@",
        "blib@",
        "mak@",
        "bar@",
        "est@",
        "nlo@",
        "kle@",
        "hoh@"
        },
        {//Orc
        "gen@",
        "nir@",
        "aar@",
        "air@",
        "olen@",
        "geth@",
        "menk@",
        "fhan@"
        },
        {//Fuzzer
        "papa@",
        "riga@",
        "gisa@",
        "huer@",
        "fath@",
        "mner@",
        "uiop@",
        "fust@"
        },
        {//Dracon
        "fir@",
        "nir@",
        "lir@",
        "kir@",
        "hir@",
        "mir@",
        "wir@",
        "zir@"
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