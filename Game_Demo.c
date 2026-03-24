#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "CODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "CODE")
//#pragma rodata-name (push, "CODE")
#endif

#if defined (__C64__)
#pragma code-name (push, "GAME")
#pragma rodata-name (push, "GAME_RODATA")
#endif

#define DefaultScreen Title
screenName currentScreen;
byte strTemp[ConsoleBufferLength];

struct Session Sessions[4];

const RaceDescriptionDef const RaceDescription[8] =
{
  {"Human", 255},
  {"Elf", 6},
  {"Dwarf", 255},
  {"Frobit", 6},
  {"Orc", 6},
  {"Fuzzer", 6},
  {"Dracon", 6},
  {"Construct", 6}
  //{"Construct", 8} // Never levels up, strong early on but expensive to upgrade stats
};


const ClassDescriptionDef const ClassDescription[8]=
{
  {"Fighter", 8},
  {"Ranger", 8},
  {"Wizard", 4},
  {"Paladin", 6},
  {"Monk", 6},
  {"Bard", 4},
  {"Thief", 4},
  {"Cleric", 6}
};

const sbyte const AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};

#if defined (__NES__)

void SelectBank()
{
    MMC3_PRG_8000(currentScreen);
}
#endif

void DrawInterface()
{
        DrawBorder("@", viewportPosX - 1, viewportPosY - 1, viewportWidth* 2 + 2, viewportHeight * 2 + 2, true);
        //DrawCharStats();
        ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
        SetTileOrigin(viewportPosX, viewportPosY);
}

void SwitchScreen(screenName screen)
{
  //ResizeMessageWindow(1, 1, 10, 15);
  //WriteLineMessageWindow("Hello@", 0);
  //ScreenDisable();
  //ClearScreen();
  //DrawInterface();
  //Load specified screen
  //UpdateInput();
  currentScreen = screen;
  //ScreenEnable();
  #if defined (__NES__)
  SelectBank();
  #endif

  switch (currentScreen)
  {
    case EditParty:
      currentScreen = DrawAddCharacterScreen();
      break;
    case Map:
      currentScreen = MapUpdate();
      break;
    case Combat:
      #if !MSX
      currentScreen = Update_Combat();
      #endif
      break;
    case MapGen:
      //#if !MSX
      currentScreen = Update_MapGen();
      //#endif
      break;
    case Scenario:
      #if !MSX
      //currentScreen = Update_Scenario();
      #endif
    default:
      currentScreen = DefaultScreen;
      break;
  }
  SwitchScreen(currentScreen);
}

void RunGame(screenName startingScreen)
{
  srand(0);
  InitializeInput();
  InitializeGraphics();
  ClearScreen();
  ScreenFadeOut();
  #if defined(__NES__)
  MMC3_PRG_8000(0);
  #endif
  LoadMap();
  currentScreen = startingScreen;
DrawInterface();

        //WriteLineMessageWindow("Greetings from the librarian Soodo Nim@", 0);
        //WriteLineMessageWindow("..not to be confused with evil Anto Nim@", 0);
        //WaitForInput();
  //DrawBorder("@", 12, 19, 16, 5, true);
  //PrintString("Press Space@", 14, 21, true);
  while(1)
  {
    SwitchScreen(currentScreen);
    //DrawBorder("@", 12, 19, 16, 5, true);
  //PrintString("Press Space@", 14, 21, true);

  while (1)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        break;
  }
  }
}

struct playerChar *startRoster;
//struct playerChar *startRoster=NULL;

byte CountRoster()
{
  struct playerChar *temp = startRoster;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

void create()
{
  struct playerChar *temp,*ptr;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));

  if(temp==NULL)
    return;

  temp->next=NULL;
  if(startRoster==NULL)
    startRoster=temp;
  else
  {
    ptr=startRoster;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
}

struct playerChar *getPlayerChar(byte index)
{
  byte i = 0;
  struct playerChar *tmp = startRoster;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void delete_pos(byte pos)
{
  byte i;
  struct playerChar *temp,*ptr;
  temp = NULL;

  if(startRoster==NULL)
    return;
  else
  {
    if(pos==0)
    {
      ptr=startRoster;
      startRoster=startRoster->next ;
    }
    else
    {
      ptr=startRoster;
      for(i=0;i<pos;i++)
      {
        temp=ptr;
        ptr=ptr->next ;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:@", 0);
          return;
        }
      }
      temp->next = ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

//Party
struct playerChar *startParty;
//struct playerChar *startParty=NULL;

byte CountParty()
{
  struct playerChar *temp = startParty;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

struct playerChar *getPartyMember(byte index)
{
  byte i = 0;
  struct playerChar *tmp = startParty;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void AddParty(byte index)
{
  struct playerChar *temp,*ptr,*src;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));
  src = getPlayerChar(index);

  if(temp==NULL)
    return;
  memcpy(temp, src, sizeof(struct playerChar));
  temp->next=NULL;
  if(startParty==NULL)
    startParty=temp;
  else
  {
    ptr=startParty;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  delete_pos(index);
}

void DeleteParty(byte pos)
{
  byte i;
  struct playerChar *temp,*ptr;
  temp = NULL;

  if(startParty==NULL)
    return;
  else
  {
    if(pos==0)
    {
      ptr=startParty;
      startParty=startParty->next ;
    }
    else
    {
      ptr=startParty;
      for(i=0;i<pos;i++)
      {
        temp=ptr;
        ptr=ptr->next ;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:@", 0);
          return;
        }
      }
      temp->next =ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

void RemoveParty(byte index) //Removes Last Party Member (?)
{
  //byte index = CountParty()-1;
  struct playerChar *temp,*ptr,*src;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));
  src = getPartyMember(index);

  if(temp==NULL)
    return;
  memcpy(temp, src, sizeof(struct playerChar));
  temp->next=NULL;
  if(startRoster==NULL)
    startRoster=temp;
  else
  {
    ptr=startRoster;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  DeleteParty(index);
}
